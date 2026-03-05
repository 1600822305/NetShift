# NetShift 工作流程文档

## 开发流程

### 环境准备

- **IDE**: Visual Studio 2022 / Rider / VS Code
- **SDK**: .NET 6.0 SDK
- **OS**: Windows 10/11 x64

### 本地开发

```bash
# 克隆仓库
git clone https://github.com/1600822305/NetShift.git
cd NetShift

# 编译（调试）
dotnet build Netch/Netch.csproj

# 发布（生产）
dotnet publish Netch/Netch.csproj -c Release -r win-x64 --self-contained ^
  -p:PublishSingleFile=true ^
  -p:IncludeNativeLibrariesForSelfExtract=true ^
  -o publish
```

### 本地测试部署

编译后将 `publish/Netch.exe` 复制到 Netch 安装目录替换即可：

```powershell
# 先关闭正在运行的 Netch
taskkill /f /im Netch.exe

# 复制到安装目录（根据实际路径调整）
Copy-Item "publish\Netch.exe" "你的Netch安装目录\Netch.exe" -Force
```

---

## 发布流程

### 自动发布（推荐）

项目使用 GitHub Actions 实现 **tag 触发自动构建发布**。

#### 步骤

1. **确认代码已提交**

```bash
git add -A
git commit -m "feat: 你的改动描述"
git push origin main
```

2. **打版本 tag**

```bash
# 格式：v主版本.次版本.修订号
git tag v1.9.8
```

3. **推送 tag 触发构建**

```bash
git push origin v1.9.8
```

4. **等待构建完成**

前往 [Actions](https://github.com/1600822305/NetShift/actions) 查看构建进度。

5. **下载发布包**

构建成功后，自动在 [Releases](https://github.com/1600822305/NetShift/releases) 生成：
- Release 页面，包含版本说明
- `NetShift-v1.9.8-x64.zip` 下载包

#### 版本号规范

| 格式 | 示例 | 说明 |
|------|------|------|
| `vX.Y.Z` | `v1.9.7` | 正式版本 |
| `vX.Y.Z-beta` | `v2.0.0-beta` | 测试版本 |

- **X（主版本）**：大功能变更或不兼容更新
- **Y（次版本）**：新增功能
- **Z（修订号）**：Bug 修复

### 手动发布

如果不想用自动构建，也可以手动操作：

```bash
# 1. 本地构建
dotnet publish Netch/Netch.csproj -c Release -r win-x64 --self-contained ^
  -p:PublishSingleFile=true ^
  -p:IncludeNativeLibrariesForSelfExtract=true ^
  -o publish

# 2. 打包
# 将 publish/ 目录下的文件打成 zip

# 3. 在 GitHub Releases 页面手动创建 Release 并上传 zip
```

---

## CI/CD 配置

### 构建流水线

文件位置：`.github/workflows/release.yml`

**触发条件**：推送 `v*` 格式的 tag

**构建步骤**：
1. Checkout 代码
2. 安装 .NET 6.0 SDK
3. 安装 MSBuild
4. `dotnet restore` 还原依赖
5. `dotnet publish` 生成单文件 exe
6. 打包成 zip
7. 创建 GitHub Release 并上传

---

## 项目结构

```
NetShift/
├── .github/workflows/    # GitHub Actions 工作流
│   └── release.yml       # Tag 触发的构建发布流程
├── Netch/                # C# 主项目
│   ├── Forms/            # WinForms 界面
│   │   ├── MainForm.cs           # 主窗口逻辑
│   │   └── MainForm.Designer.cs  # 主窗口 UI 定义
│   ├── Models/           # 数据模型
│   │   └── Profile.cs            # 配置档案（含 ServerType）
│   ├── Servers/          # 服务器类型实现
│   │   └── Http/                 # HTTP 代理（新增）
│   ├── Controllers/      # 控制器
│   └── Netch.csproj      # 项目文件
├── Redirector/           # C++ 流量重定向器
│   ├── HttpHelper.cpp    # HTTP 代理辅助（新增）
│   ├── HttpHelper.h
│   └── TCPHandler.cpp    # TCP 处理（含 HTTP 支持）
├── README.md             # 项目介绍
├── WORKFLOW.md           # 本文档
└── LICENSE               # GPL-2.0 许可证
```

---

## 常见问题

### Q: tag 推送后 Actions 没有触发？
确认 tag 格式以 `v` 开头，例如 `v1.9.8`。

### Q: 构建失败怎么办？
前往 [Actions](https://github.com/1600822305/NetShift/actions) 查看日志，常见原因：
- NuGet 包还原失败（网络问题，重新运行即可）
- 代码编译错误（先本地 `dotnet build` 确认无误再推送）

### Q: 如何删除错误的 tag？
```bash
# 删除本地 tag
git tag -d v1.9.8

# 删除远程 tag
git push origin :refs/tags/v1.9.8
```

### Q: 如何修改版本号？
修改 `Netch/Controllers/UpdateChecker.cs` 中的 `AssemblyVersion`：
```csharp
public const string AssemblyVersion = @"1.9.8";
```
