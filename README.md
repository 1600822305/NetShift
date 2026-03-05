<div align="center">

# NetShift

基于 [Netch](https://github.com/netchx/netch) 的定制化网络代理加速工具

[![GitHub release](https://img.shields.io/github/v/release/1600822305/NetShift?style=flat-square)](https://github.com/1600822305/NetShift/releases)
[![GitHub downloads](https://img.shields.io/github/downloads/1600822305/NetShift/total?style=flat-square)](https://github.com/1600822305/NetShift/releases)
[![License](https://img.shields.io/github/license/1600822305/NetShift?style=flat-square)](LICENSE)

</div>

## 简介

NetShift 是基于 [Netch](https://github.com/netchx/netch)（v1.9.7）二次开发的网络代理加速工具。在保留原版全部功能的基础上，新增了 **HTTP 代理支持** 和 **快速配置切换** 等实用功能。

## 相比原版 Netch 的改动

### 新增功能

- **HTTP 代理服务器支持** — 新增 HTTP 代理类型，可直接添加 HTTP 代理服务器（C++ Redirector + C# 双端实现）
- **快速配置切换** — 一键保存当前服务器+模式为配置，点击即可切换并启动
- **配置管理** — 最多保存 8 个配置，右键删除，激活配置高亮显示
- **精确配置恢复** — 保存配置时记录服务器类型（ServerType），恢复时精确匹配 HTTP/SOCKS5 服务器

### UI 改进

- 去掉了原版的配置名输入框，改用弹窗输入
- 配置按钮使用 FlowLayoutPanel 流式布局，自动排列换行
- 新增"保存配置"文字按钮，操作更直观

## 原版功能

### 代理模式
- **ProcessMode** — 使用 Netfilter 驱动拦截进程流量
- **ShareMode** — 基于 WinPcap/Npcap 共享网络
- **TunMode** — 使用 WinTUN 驱动创建虚拟网卡
- **WebMode** — Web 代理模式

### 支持协议
- [Socks5](https://www.wikiwand.com/en/SOCKS)
- [Shadowsocks](https://shadowsocks.org)
- [ShadowsocksR](https://github.com/shadowsocksrr/shadowsocksr-libev)
- [WireGuard](https://www.wireguard.com)
- [Trojan](https://trojan-gfw.github.io/trojan)
- [VMess](https://www.v2fly.org)
- [VLESS](https://xtls.github.io)
- **HTTP**（NetShift 新增）

### 其他
- UDP NAT FullCone（取决于服务器支持）
- .NET 6.0 x64

## 安装使用

### 下载
前往 [Releases](https://github.com/1600822305/NetShift/releases) 下载最新版本的 `NetShift-vX.X.X-x64.zip`。

### 系统要求
- Windows 10/11 x64
- .NET 6.0 Runtime（已内置，无需额外安装）

### 使用步骤
1. 解压到任意目录
2. 运行 `Netch.exe`
3. 添加服务器（支持 SOCKS5、HTTP、Shadowsocks 等）
4. 选择模式
5. 点击"启动"

### 快速配置
1. 选好服务器和模式后，点击"保存配置"按钮
2. 输入配置名称，确认保存
3. 下次直接点击配置按钮，一键切换并启动
4. 右键配置按钮可删除该配置

## 本地构建

```bash
# 克隆仓库
git clone https://github.com/1600822305/NetShift.git
cd NetShift

# 构建
dotnet publish Netch/Netch.csproj -c Release -r win-x64 --self-contained \
  -p:PublishSingleFile=true \
  -p:IncludeNativeLibrariesForSelfExtract=true \
  -o publish
```

构建产物在 `publish/` 目录下。

## 发布新版本

详见 [WORKFLOW.md](WORKFLOW.md)。

## 致谢

- [Netch](https://github.com/netchx/netch) — 原版项目，本项目基于其 v1.9.7 版本二次开发
- [JetBrains](https://www.jetbrains.com/?from=Netch) — 开发工具支持

## 许可证

本项目基于 [GPL-2.0](LICENSE) 许可证开源，继承自原版 Netch 项目。
