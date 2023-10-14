#!/bin/sh

echo '正在安装/升级必需插件. . .'

apt update

apt install -y curl wget tar gawk sed

echo '正在安装/升级go. . .'

apt remove -y --purge golang

apt autoremove -y

rm -rf /usr/local/go

Go_Version=$(curl https://github.com/golang/go/tags | grep '/releases/tag/go' | head -n 1 | gawk -F/ '{print $6}' | gawk -F\" '{print $1}')

#Go_Version=$(curl https://go.dev/dl/ | grep download | gawk -F'>' '{print $3}' | gawk -F'<' '{print $1}' | grep linux-amd64 | head -n 1 | sed -e "s/go//g" | sed -e "s@.linux-amd64.tar.gz@@g")

wget -O /var/tmp/${Go_Version}.linux-amd64.tar.gz https://go.dev/dl/${Go_Version}.linux-amd64.tar.gz

tar -C /usr/local -xzf /var/tmp/${Go_Version}.linux-amd64.tar.gz

rm -f /var/tmp/${Go_Version}.linux-amd64.tar.gz

export PATH=$PATH:/usr/local/go/bin

go version

echo '正在编译Windows版lux核心. . .'

lux_Version=$(curl https://api.github.com/repos/iawia002/lux/releases/latest  | grep "tag_name" | gawk -F\" '{print $4}')

env GOOS=windows GOARCH=amd64 GOAMD64=v3 CGO_ENABLED=0 go install -ldflags "-X github.com/iawia002/lux/app.version=${lux_Version}" github.com/iawia002/lux@latest

echo 'lux核心已编译!'

exit 0
