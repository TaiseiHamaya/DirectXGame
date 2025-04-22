﻿# --- ステップ0: プロジェクト名を取得 ---
$ProjectName = Read-Host "プロジェクト名を入力してください"

# --- ステップ1: プロジェクト名のフォルダを作成 ---
$projectRoot = Join-Path -Path "." -ChildPath $ProjectName
if (-not (New-Item -ItemType Directory -Path $projectRoot -ErrorAction SilentlyContinue)) {
    Write-Host "フォルダの作成に失敗しました。"
    Read-Host "Enterキーで終了"
    exit 1
}

# --- ステップ2: projectフォルダを作成 ---
$projectSubdir = Join-Path -Path $projectRoot -ChildPath "project"
if (-not (New-Item -ItemType Directory -Path $projectSubdir -ErrorAction SilentlyContinue)) {
    Write-Host "projectフォルダの作成に失敗しました。"
    Read-Host "Enterキーで終了"
    exit 1
}

# --- ステップ3: gitリポジトリを初期化 ---
Push-Location $projectSubdir
git init
if ($LASTEXITCODE -ne 0) {
    Write-Host "git初期化に失敗しました。"
    Pop-Location
    Read-Host "Enterキーで終了"
    exit 1
}

# --- ステップ4: サブモジュールの追加 ---
git submodule add https://github.com/TaiseiHamaya/DirectXGame.git
if ($LASTEXITCODE -ne 0) {
    Write-Host "サブモジュールの追加に失敗しました。"
    Pop-Location
    Read-Host "Enterキーで終了"
    exit 1
}

# --- ステップ5: CopyFolderRootフォルダの内容をコピー ---
$sourcePath = ".\DirectXGame\ProjectGeneratorTool\CopyFolderRoot\*"
$destinationPath = ".\"  # 現在の場所は project/
try {
    Copy-Item -Path $sourcePath -Destination $destinationPath -Recurse -Force -ErrorAction Stop
} catch {
    Write-Host "コピーに失敗しました。エラー内容:"
    Write-Host $_.Exception.Message
    Pop-Location
    Read-Host "Enterキーで終了"
    exit 1
}

# --- ステップ6: __Template__.sln のファイル名と内容を置換 ---
$templateSlnPath = Join-Path -Path $destinationPath -ChildPath "__Template__.sln"
if (Test-Path $templateSlnPath) {
    $newSlnPath = Join-Path -Path $destinationPath -ChildPath "$ProjectName.sln"

    # ファイル名の変更
    Rename-Item -Path $templateSlnPath -NewName "$ProjectName.sln"

    # ファイル内容の置換
    (Get-Content -Path $newSlnPath -Raw -Encoding UTF8) -replace '__Template__', $ProjectName |
        Set-Content -Path $newSlnPath -Encoding UTF8
}

# --- ステップ7: Gameフォルダ内のファイルに対する処理 ---
# --- ステップ7: Gameフォルダ内の処理 ---
$gameDir = Join-Path -Path $destinationPath -ChildPath "Game"
if (Test-Path $gameDir) {
    # ① フォルダ名の __Template__ 置換（深い順から処理）
    Get-ChildItem -Path $gameDir -Recurse -Directory |
        Sort-Object FullName -Descending |
        ForEach-Object {
            if ($_.Name -like "*__Template__*") {
                $newName = $_.Name -replace '__Template__', $ProjectName
                Rename-Item -Path $_.FullName -NewName $newName
            }
        }

    # ② ファイルの処理（ファイル名・内容・拡張子）
    Get-ChildItem -Path $gameDir -Recurse -File | ForEach-Object {
        $file = $_

        # 拡張子 .template を削除
        if ($file.Name -match "\.template$") {
            $newName = $file.Name -replace '\.template$', ''
            $file = Rename-Item -Path $file.FullName -NewName $newName -PassThru
        }

        # ファイル名の __Template__ を置換
        if ($file.Name -like "*__Template__*") {
            $newName = $file.Name -replace '__Template__', $ProjectName
            $file = Rename-Item -Path $file.FullName -NewName $newName -PassThru
        }

        # ファイル内容の __Template__ を置換（.exe は除外）
        if ($file.Extension -ne ".exe") {
            (Get-Content -Path $file.FullName -Raw -Encoding UTF8) -replace '__Template__', $ProjectName |
                Set-Content -Path $file.FullName -Encoding UTF8
        }
    }
}


Pop-Location
Write-Host "完了しました。"
Read-Host "Enterキーで終了"
