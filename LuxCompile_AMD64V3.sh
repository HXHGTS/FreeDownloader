#!/bin/sh

#第一次使用请先运行 curl -sSL https://raw.githubusercontent.com/HXHGTS/Singbox-Install/main/sh/go_install.sh | bash 安装Go环境!

echo '正在打开Go环境. . .'

source /root/.bashrc

go version

echo '正在编译Windows版lux核心. . .'

lux_Version=$(curl https://api.github.com/repos/iawia002/lux/releases/latest  | grep "tag_name" | gawk -F\" '{print $4}')

env GOOS=windows GOARCH=amd64 GOAMD64=v3 CGO_ENABLED=0 go install -ldflags "-X github.com/iawia002/lux/app.version=${lux_Version}" github.com/iawia002/lux@latest

echo 'lux核心已编译!'

exit 0
