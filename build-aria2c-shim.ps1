#Requires -Version 5.1
<#
    build-aria2c-shim.ps1  -  compile aria2c.exe, the self-contained aria2
                              compatibility shim

    Produces aria2c.exe next to this script.  The shim is SELF-CONTAINED: all
    of the classic-aria2 -> aria2-next argument rewriting lives in the C# below,
    so there is no aria2.ps1, no PowerShell hop and no other runtime file.

        anything that says "aria2c"  ->  aria2c.exe  ->  aria2-next.exe

    WHY aria2c.exe HAS TO EXIST AT ALL
        FreeDownloader.exe hard-codes four aria2c call sites that cannot be
        changed from outside the binary:
            temp\rpc.bat        start /min aria2c --conf-path=config\rpc.conf
            version check       aria2c -v
            DHT probe           aria2c -s16 -x16 -k1M ... --out=dht.dat "URL"
            cleanup             taskkill /f /im aria2c*
        Deleting it would break all four; renaming the engine to aria2-next.exe
        (its upstream name) left this name free, so the shim takes it.

    KILL SEMANTICS
        The engine child is put in a kill-on-close job object, so
        `taskkill /f /im aria2c*` tears the whole tree down instead of leaving
        an orphaned RPC daemon holding port 6800.

    ENVIRONMENT (same names the old aria2.ps1 shim used)
        ARIA2_COMPAT_NOTES=1   print what was rewritten / dropped, to stderr
        ARIA2_COMPAT_DRYRUN=1  print the rewritten command line and exit 0
        ARIA2_NEXT_BIN         explicit path to the engine

    REBUILD CONTRACT
        Windows PowerShell 5.1's Add-Type uses the in-box .NET Framework C#
        compiler, so no SDK, no network and no compiler install are needed.
        The result is a ~10 KB console exe running on any Windows with
        .NET Framework 4.x.

    USAGE
        powershell -NoProfile -ExecutionPolicy Bypass -File build-aria2c-shim.ps1
#>

$ErrorActionPreference = 'Stop'

$target = Join-Path $PSScriptRoot 'aria2c.exe'

$source = @'
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;

internal static class Aria2cShim
{
    // ===================================================================
    // Tables.  [A] and [B] mirror aria2-next src/options/Options.cc
    // (retired / retiredFlags) plus the Flag entries of its Catalog.cc, so
    // the shim can predict exactly what the engine would warn about.
    // ===================================================================

    // [A] No counterpart in aria2-next: the engine warns "retired; ignored",
    //     so we drop them (unless a rule below maps them to a replacement).
    private static readonly HashSet<string> Retired =
        new HashSet<string>(StringComparer.Ordinal)
    {
        "allow-piece-length-change", "always-resume", "async-dns",
        "async-dns-server", "bt-announce-all-tiers", "bt-announce-all-trackers",
        "bt-checking-memory", "bt-connection-speed", "bt-detach-seed-only",
        "bt-disk-io", "bt-disk-queue-size", "bt-disk-read-cache",
        "bt-disk-write-cache", "bt-enable-hook-after-hash-check",
        "bt-hash-check-seed", "bt-hashing-threads", "bt-io-threads",
        "bt-keep-alive-interval", "bt-load-saved-metadata", "bt-lpd-interface",
        "bt-max-concurrent-http-announces", "bt-max-in-request-queue",
        "bt-max-open-files", "bt-max-out-request-queue", "bt-metadata-only",
        "bt-min-crypto-level", "bt-mixed-mode", "bt-peer-turnover",
        "bt-peer-turnover-cutoff", "bt-peer-turnover-interval",
        "bt-piece-extent-affinity", "bt-prioritize-piece",
        "bt-remove-unselected-file", "bt-request-peer-speed-limit",
        "bt-request-timeout", "bt-resume-save-interval", "bt-save-metadata",
        "bt-seed-choking-algorithm", "bt-seeding-outgoing-connections",
        "bt-send-buffer-low-watermark", "bt-send-buffer-watermark",
        "bt-send-buffer-watermark-factor", "bt-stop-timeout",
        "bt-stop-tracker-timeout", "bt-timeout", "bt-tracker-completion-timeout",
        "bt-tracker-connect-timeout", "bt-tracker-interval",
        "bt-tracker-receive-timeout", "bt-tracker-timeout",
        "bt-upload-slot-algorithm", "bt-upload-suggestions", "conditional-get",
        "content-disposition-default-utf8", "deferred-input",
        "dht-entry-point", "dht-entry-point-host", "dht-entry-point-host6",
        "dht-entry-point-port", "dht-entry-point-port6", "dht-entry-point6",
        "dht-file-path", "dht-file-path6", "dht-listen-addr",
        "dht-listen-addr6", "dht-listen-port", "dht-message-timeout",
        "disk-cache", "dscp", "ed2k-min-split-size", "ed2k-piece-selector",
        "enable-async-dns6", "enable-dht6", "enable-http-pipelining",
        "enable-mmap", "event-poll", "ftp-passwd", "ftp-pasv", "ftp-proxy",
        "ftp-proxy-passwd", "ftp-proxy-user", "ftp-reuse-connection",
        "ftp-type", "ftp-user", "http-auth-challenge", "human-readable",
        "max-http-pipelining", "max-mmap-limit", "metalink-base-uri",
        "metalink-enable-unique-protocol", "metalink-language",
        "metalink-location", "metalink-os", "metalink-preferred-protocol",
        "metalink-version", "multiple-interface", "no-file-allocation-limit",
        "no-want-digest-header", "optimize-concurrent-downloads",
        "parameterized-uri", "peer-agent", "peer-connection-timeout",
        "peer-id-prefix", "piece-length", "proxy-method",
        "realtime-chunk-checksum", "reuse-uri", "rlimit-nofile",
        "rpc-certificate", "rpc-passwd", "rpc-private-key", "rpc-secure",
        "rpc-user", "select-least-used-host", "server-stat-if",
        "server-stat-of", "server-stat-timeout", "socket-recv-buffer-size",
        "ssh-host-key-md", "startup-idle-time", "stderr",
        "stream-max-range-size", "stream-piece-selector",
        "truncate-console-readout", "uri-selector", "use-head"
    };

    // [B] Options that take NO value when written in the space-separated form,
    //     i.e. "--continue" is a bare flag but "--dir" eats the next token.
    private static readonly HashSet<string> NoValue =
        new HashSet<string>(StringComparer.Ordinal)
    {
        "allow-overwrite", "allow-piece-length-change", "always-resume",
        "async-dns", "auto-file-renaming", "bt-announce-all-tiers",
        "bt-announce-all-trackers", "bt-anonymous-mode", "bt-detach-seed-only",
        "bt-enable-hook-after-hash-check", "bt-enable-lpd",
        "bt-first-last-piece-first", "bt-force-encryption",
        "bt-hash-check-seed", "bt-load-saved-metadata", "bt-metadata-only",
        "bt-piece-extent-affinity", "bt-port-mapping", "bt-rate-limit-overhead",
        "bt-remove-unselected-file", "bt-require-crypto", "bt-save-metadata",
        "bt-seeding-outgoing-connections", "bt-seed-unverified",
        "bt-super-seeding", "bt-upload-suggestions", "check-certificate",
        "check-integrity", "conditional-get",
        "content-disposition-default-utf8", "continue", "daemon",
        "deferred-input", "detach-share-only", "disable-ipv6", "dry-run",
        "ed2k-port-mapping", "ed2k-preview-priority", "enable-async-dns6",
        "enable-color", "enable-dht", "enable-dht6", "enable-http-keep-alive",
        "enable-http-pipelining", "enable-mmap", "enable-peer-exchange",
        "enable-rpc", "force-save", "force-sequential", "ftp-pasv",
        "ftp-reuse-connection", "hash-check-only", "http-accept-gzip",
        "http-auth-challenge", "http-no-cache", "human-readable",
        "keep-unfinished-download-result", "media-pause-after-probe",
        "metalink-enable-unique-protocol", "no-conf", "no-netrc",
        "no-want-digest-header", "optimize-concurrent-downloads",
        "parameterized-uri", "pause", "pause-metadata", "quiet",
        "realtime-chunk-checksum", "remote-time", "remove-control-file",
        "reuse-uri", "rpc-allow-origin-all", "rpc-listen-all",
        "rpc-save-upload-metadata", "rpc-secure", "save-not-found",
        "select-least-used-host", "show-console-readout", "show-files",
        "stderr", "truncate-console-readout", "use-head", "version"
    };

    // [C] Classic short options.  Case matters (-d dir vs -D daemon), so this
    //     dictionary uses ordinal comparison.
    private static readonly Dictionary<string, string> ShortMap =
        new Dictionary<string, string>(StringComparer.Ordinal)
    {
        { "c", "continue"                   }, { "d", "dir" },
        { "D", "daemon"                     }, { "h", "help" },
        { "i", "input-file"                 }, { "j", "max-concurrent-downloads" },
        { "k", "min-split-size"             }, { "l", "log" },
        { "M", "metalink-file"              }, { "m", "max-tries" },
        { "n", "no-netrc"                   }, { "O", "index-out" },
        { "o", "out"                        }, { "P", "parameterized-uri" },
        { "p", "ftp-pasv"                   }, { "q", "quiet" },
        { "R", "remote-time"                }, { "S", "show-files" },
        { "s", "split"                      }, { "T", "torrent-file" },
        { "t", "timeout"                    }, { "U", "user-agent" },
        { "u", "max-upload-limit"           }, { "V", "check-integrity" },
        { "v", "version"                    }, { "x", "max-connection-per-server" },
        { "Z", "force-sequential" }
    };

    private sealed class Rule
    {
        public string[] Targets;   // option names that receive the value
        public string Const;       // literal name=value emitted instead
        public int Mode;           // 0 forward, 1 truthy, 2 not-false, 3 equals
        public string EqValue;     // for Mode 3
        public bool FirstOnly;     // forward only the first comma-separated entry
        public string Strip;       // strip this prefix before forwarding
        public bool Dead;          // no counterpart, not even in the retired table
    }

    // [D] Classic option -> aria2-next replacement.
    private static readonly Dictionary<string, Rule> Map =
        new Dictionary<string, Rule>(StringComparer.Ordinal)
    {
        { "stream-max-connections",
            new Rule{ Targets = new[]{ "split", "max-connection-per-server" } } },
        { "auto-save-interval",
            new Rule{ Targets = new[]{ "state-save-interval" } } },
        { "bt-resume-save-interval",
            new Rule{ Targets = new[]{ "state-save-interval" } } },
        { "bt-detach-seed-only",
            new Rule{ Targets = new[]{ "detach-share-only" } } },
        { "peer-agent",
            new Rule{ Targets = new[]{ "bt-user-agent" } } },
        { "peer-id-prefix",
            new Rule{ Targets = new[]{ "bt-peer-id-prefix" } } },
        { "dht-entry-point",
            new Rule{ Targets = new[]{ "bt-dht-bootstrap-nodes" } } },
        { "dht-entry-point6",
            new Rule{ Targets = new[]{ "bt-dht-bootstrap-nodes" } } },
        { "multiple-interface",
            new Rule{ Targets = new[]{ "interface" }, FirstOnly = true } },
        { "ssh-host-key-md",
            new Rule{ Targets = new[]{ "ssh-host-key-sha256" }, Strip = "sha-256=" } },
        { "bt-require-crypto",
            new Rule{ Const = "bt-encryption=required", Mode = 1 } },
        { "bt-force-encryption",
            new Rule{ Const = "bt-encryption=required", Mode = 1 } },
        { "bt-prioritize-piece",
            new Rule{ Const = "bt-first-last-piece-first=true", Mode = 2 } },
        { "bt-min-crypto-level",
            new Rule{ Const = "bt-encryption=required", Mode = 3, EqValue = "arc4" } },
        { "max-resume-failure-tries",
            new Rule{ Dead = true } }
    };

    // [E] Integer ranges enforced by aria2-next's catalogue: a value aria2
    //     accepted but aria2-next rejects would abort the run, so clamp.
    private static readonly Dictionary<string, long[]> Clamp =
        new Dictionary<string, long[]>(StringComparer.Ordinal)
    {
        { "bt-max-connections",        new long[]{ 2, 65535 } },
        { "bt-max-uploads",            new long[]{ 1, 65535 } },
        { "bt-max-uploads-per-torrent",new long[]{ 1, 65535 } },
        { "max-concurrent-downloads",  new long[]{ 1, 1000 } },
        { "split",                     new long[]{ 1, 256 } },
        { "max-connection-per-server", new long[]{ 1, 256 } },
        { "state-save-interval",       new long[]{ 1, 600 } },
        { "timeout",                   new long[]{ 1, 600 } },
        { "connect-timeout",           new long[]{ 1, 600 } },
        { "retry-wait",                new long[]{ 0, 600 } },
        { "log-max-files",             new long[]{ 1, 64 } }
    };

    // ===================================================================
    // Translation state
    // ===================================================================
    private static readonly List<string> OutArgs = new List<string>();
    private static readonly List<string> Notes = new List<string>();
    private static bool notesEnabled;

    private static void AddNote(string text)
    {
        if (notesEnabled) { Notes.Add(text); }
    }

    private static bool IsTruthy(string v)
    {
        if (string.IsNullOrEmpty(v)) { return true; }
        switch (v.ToLowerInvariant()) {
            case "true": case "1": case "yes": case "on": return true;
            default: return false;
        }
    }

    // For options whose value is a mode rather than a boolean: anything that is
    // not an explicit false is "on" (--bt-prioritize-piece=head / head:1M).
    private static bool IsNotFalse(string v)
    {
        if (string.IsNullOrEmpty(v)) { return false; }
        switch (v.ToLowerInvariant()) {
            case "false": case "0": case "no": case "off": return false;
            default: return true;
        }
    }

    private static string NormalizeValue(string name, string value)
    {
        if (name == "log-level" || name == "console-log-level") {
            if (value == "warning") { AddNote("--" + name + "=" + value + " -> warn"); return "warn"; }
            if (value == "trace")   { AddNote("--" + name + "=" + value + " -> debug"); return "debug"; }
            return value;
        }
        if (name == "listen-port") {
            int cut = value.IndexOf('-');
            int comma = value.IndexOf(',');
            if (comma >= 0 && (cut < 0 || comma < cut)) { cut = comma; }
            if (cut >= 0) {
                string head = value.Substring(0, cut);
                AddNote("--listen-port=" + value + " -> " + head + " (ranges not supported)");
                return head;
            }
            return value;
        }
        long[] range;
        if (Clamp.TryGetValue(name, out range)) {
            long parsed;
            if (long.TryParse(value, NumberStyles.Integer,
                              CultureInfo.InvariantCulture, out parsed)) {
                if (parsed < range[0]) {
                    AddNote("--" + name + "=" + value + " -> " + range[0] +
                            " (below aria2-next minimum)");
                    return range[0].ToString();
                }
                if (parsed > range[1]) {
                    AddNote("--" + name + "=" + value + " -> " + range[1] +
                            " (above aria2-next maximum)");
                    return range[1].ToString();
                }
                return parsed.ToString();
            }
        }
        return value;
    }

    // Emit one option in the unambiguous --name=value form.
    private static void EmitOption(string name, string value, bool isFlag)
    {
        if (isFlag) {
            if (string.IsNullOrEmpty(value) || value == "true") {
                OutArgs.Add("--" + name);
            } else {
                OutArgs.Add("--" + name + "=" + value);
            }
            return;
        }
        OutArgs.Add("--" + name + "=" + NormalizeValue(name, value));
    }

    private static void ConvertOption(string name, string value)
    {
        Rule rule;
        if (Map.TryGetValue(name, out rule)) {
            if (rule.Dead) {
                AddNote("--" + name + " dropped (not implemented by aria2-next)");
                return;
            }
            if (rule.Const != null) {
                bool emit;
                if (rule.Mode == 1)      { emit = IsTruthy(value); }
                else if (rule.Mode == 2) { emit = IsNotFalse(value); }
                else                     { emit = value == rule.EqValue; }
                if (emit) { OutArgs.Add("--" + rule.Const); }
                else {
                    AddNote("--" + name + "=" + value +
                            " dropped (already the aria2-next default)");
                }
                return;
            }
            string v = value;
            if (rule.FirstOnly) {
                int comma = v.IndexOf(',');
                if (comma >= 0) {
                    AddNote("--" + name + "=" + v + " -> " + v.Substring(0, comma) +
                            " (only the first interface)");
                    v = v.Substring(0, comma);
                }
            }
            if (rule.Strip != null) {
                if (v.StartsWith(rule.Strip, StringComparison.Ordinal)) {
                    v = v.Substring(rule.Strip.Length);
                } else {
                    AddNote("--" + name + "=" + value + " dropped (only " +
                            rule.Strip + " can be converted)");
                    return;
                }
            }
            foreach (string target in rule.Targets) {
                if (target != name) { AddNote("--" + name + " -> --" + target); }
                EmitOption(target, v, NoValue.Contains(target));
            }
            return;
        }

        if (Retired.Contains(name)) {
            AddNote("--" + name + " dropped (retired in aria2-next)");
            return;
        }

        EmitOption(name, value, NoValue.Contains(name));
    }

    // Mirrors aria2-next src/options/CommandLine.cc: --name=value, --name
    // value, bare flags, short clusters, "--" end-of-options.
    private static void Translate(string[] args)
    {
        int i = 0;
        while (i < args.Length) {
            string token = args[i];

            if (token == "--") {
                for (; i < args.Length; i++) { OutArgs.Add(args[i]); }
                break;
            }

            if (token.StartsWith("--", StringComparison.Ordinal)) {
                int equals = token.IndexOf('=');
                string name;
                string value;
                if (equals >= 0) {
                    name = token.Substring(2, equals - 2);
                    value = token.Substring(equals + 1);
                } else {
                    name = token.Substring(2);
                    // --help never consumes the next token; it defaults to #basic.
                    if (name == "help") {
                        value = "#basic";
                    } else if (NoValue.Contains(name)) {
                        value = "true";
                    } else if (i + 1 < args.Length) {
                        i++;
                        value = args[i];
                    } else {
                        value = "";
                    }
                }
                ConvertOption(name, value);
                i++;
                continue;
            }

            if (token.Length > 1 && token[0] == '-') {
                string cluster = token.Substring(1);
                int k = 0;
                while (k < cluster.Length) {
                    string longName;
                    if (!ShortMap.TryGetValue(cluster[k].ToString(), out longName)) {
                        // Unknown short: hand the token over untouched, the
                        // engine reports it.
                        OutArgs.Add(token);
                        break;
                    }
                    if (NoValue.Contains(longName)) {
                        ConvertOption(longName, "true");
                        k++;
                        continue;
                    }
                    if (longName == "help") {
                        if (k + 1 < cluster.Length) {
                            ConvertOption("help", cluster.Substring(k + 1));
                        } else {
                            ConvertOption("help", "#basic");
                        }
                        break;
                    }
                    if (k + 1 < cluster.Length) {
                        ConvertOption(longName, cluster.Substring(k + 1));
                    } else if (i + 1 < args.Length) {
                        i++;
                        ConvertOption(longName, args[i]);
                    } else {
                        ConvertOption(longName, "");
                    }
                    break;
                }
                i++;
                continue;
            }

            OutArgs.Add(token);
            i++;
        }
    }

    // ===================================================================
    // Engine lookup and process plumbing
    // ===================================================================
    private static string FindEngine()
    {
        string env = Environment.GetEnvironmentVariable("ARIA2_NEXT_BIN");
        if (!string.IsNullOrEmpty(env) && File.Exists(env)) {
            return Path.GetFullPath(env);
        }
        string directory = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
        if (string.IsNullOrEmpty(directory)) { directory = Environment.CurrentDirectory; }
        string local = Path.Combine(directory, "aria2-next.exe");
        if (File.Exists(local)) { return local; }

        string pathVariable = Environment.GetEnvironmentVariable("PATH");
        if (pathVariable != null) {
            foreach (string entry in pathVariable.Split(';')) {
                string trimmed = entry.Trim();
                if (trimmed.Length == 0) { continue; }
                string candidate;
                try { candidate = Path.Combine(trimmed, "aria2-next.exe"); }
                catch { continue; }
                if (File.Exists(candidate)) { return candidate; }
            }
        }
        return null;
    }

    private const int JobObjectExtendedLimitInformation = 9;
    private const uint JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE = 0x00002000;

    [StructLayout(LayoutKind.Sequential)]
    private struct JOBOBJECT_BASIC_LIMIT_INFORMATION
    {
        public long PerProcessUserTimeLimit;
        public long PerJobUserTimeLimit;
        public uint LimitFlags;
        public UIntPtr MinimumWorkingSetSize;
        public UIntPtr MaximumWorkingSetSize;
        public uint ActiveProcessLimit;
        public UIntPtr Affinity;
        public uint PriorityClass;
        public uint SchedulingClass;
    }

    [StructLayout(LayoutKind.Sequential)]
    private struct IO_COUNTERS
    {
        public ulong ReadOperationCount;
        public ulong WriteOperationCount;
        public ulong OtherOperationCount;
        public ulong ReadTransferCount;
        public ulong WriteTransferCount;
        public ulong OtherTransferCount;
    }

    [StructLayout(LayoutKind.Sequential)]
    private struct JOBOBJECT_EXTENDED_LIMIT_INFORMATION
    {
        public JOBOBJECT_BASIC_LIMIT_INFORMATION BasicLimitInformation;
        public IO_COUNTERS IoInfo;
        public UIntPtr ProcessMemoryLimit;
        public UIntPtr JobMemoryLimit;
        public UIntPtr PeakProcessMemoryUsed;
        public UIntPtr PeakJobMemoryUsed;
    }

    [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
    private static extern IntPtr CreateJobObject(IntPtr attributes, string name);

    [DllImport("kernel32.dll", SetLastError = true)]
    private static extern bool SetInformationJobObject(IntPtr job, int infoClass, IntPtr info, uint length);

    [DllImport("kernel32.dll", SetLastError = true)]
    private static extern bool AssignProcessToJobObject(IntPtr job, IntPtr process);

    [DllImport("kernel32.dll", SetLastError = true)]
    private static extern bool CloseHandle(IntPtr handle);

    // A job that kills every member when its last handle closes, so taskkill of
    // this shim takes aria2-next.exe down too instead of orphaning the daemon.
    private static IntPtr CreateKillOnCloseJob()
    {
        IntPtr job = CreateJobObject(IntPtr.Zero, null);
        if (job == IntPtr.Zero) { return IntPtr.Zero; }
        var info = new JOBOBJECT_EXTENDED_LIMIT_INFORMATION();
        info.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
        int size = Marshal.SizeOf(typeof(JOBOBJECT_EXTENDED_LIMIT_INFORMATION));
        IntPtr buffer = Marshal.AllocHGlobal(size);
        try {
            Marshal.StructureToPtr(info, buffer, false);
            if (!SetInformationJobObject(job, JobObjectExtendedLimitInformation,
                                         buffer, (uint)size)) {
                CloseHandle(job);
                return IntPtr.Zero;
            }
        } finally {
            Marshal.FreeHGlobal(buffer);
        }
        return job;
    }

    // Rebuild one argv entry as CreateProcess command-line text, using the same
    // rules CommandLineToArgvW parses back out.
    private static string Quote(string argument)
    {
        if (argument.Length > 0 && argument.IndexOfAny(new[] { ' ', '\t', '"' }) < 0) {
            return argument;
        }
        var text = new StringBuilder();
        text.Append('"');
        int backslashes = 0;
        foreach (char c in argument) {
            if (c == '\\') { backslashes++; continue; }
            if (c == '"') {
                text.Append('\\', backslashes * 2 + 1);
                text.Append('"');
            } else {
                text.Append('\\', backslashes);
                text.Append(c);
            }
            backslashes = 0;
        }
        text.Append('\\', backslashes * 2);
        text.Append('"');
        return text.ToString();
    }

    public static int Main(string[] args)
    {
        bool dryRun = Environment.GetEnvironmentVariable("ARIA2_COMPAT_DRYRUN") == "1";
        notesEnabled = Environment.GetEnvironmentVariable("ARIA2_COMPAT_NOTES") == "1";

        Translate(args);

        if (notesEnabled && Notes.Count > 0) {
            Console.Error.WriteLine("aria2-compat: " + Notes.Count + " change(s):");
            foreach (string note in Notes) { Console.Error.WriteLine("  " + note); }
        }
        if (dryRun) {
            Console.WriteLine(string.Join(" ", OutArgs.ToArray()));
            return 0;
        }

        string engine = FindEngine();
        if (engine == null) {
            Console.Error.WriteLine(
                "aria2-compat: cannot find aria2-next.exe (set ARIA2_NEXT_BIN)");
            return 127;
        }

        var commandLine = new StringBuilder();
        foreach (string argument in OutArgs) {
            if (commandLine.Length > 0) { commandLine.Append(' '); }
            commandLine.Append(Quote(argument));
        }

        var startInfo = new ProcessStartInfo(engine, commandLine.ToString());
        startInfo.UseShellExecute = false;

        IntPtr job = CreateKillOnCloseJob();
        int code;
        using (Process child = Process.Start(startInfo)) {
            if (child == null) {
                Console.Error.WriteLine("aria2-compat: cannot start " + engine);
                return 127;
            }
            if (job != IntPtr.Zero) { AssignProcessToJobObject(job, child.Handle); }
            child.WaitForExit();
            code = child.ExitCode;
        }
        if (job != IntPtr.Zero) { CloseHandle(job); }
        return code;
    }
}
'@

if (Test-Path -LiteralPath $target) { Remove-Item -LiteralPath $target -Force }

Add-Type -TypeDefinition $source `
         -OutputAssembly $target `
         -OutputType ConsoleApplication `
         -Language CSharp `
         -ReferencedAssemblies @('System.dll', 'System.Core.dll')

if (-not (Test-Path -LiteralPath $target)) {
    throw "compile produced no $target"
}

Write-Output "built $target ($((Get-Item -LiteralPath $target).Length) bytes)"
