@echo off

setlocal EnableDelayedExpansion

chcp 65001 > nul

rem === ステップ0: 標準入力からプロジェクト名を取得 ===
set /p ProjectName=プロジェクト名を入力してください:

rem === ステップ1: プロジェクト名のフォルダを作成 ===
mkdir "%ProjectName%"
if errorlevel 1 (
	echo フォルダの作成に失敗しました。
	pause
	exit /b 1
)

rem === ステップ2: projectフォルダを作成 ===
mkdir "%ProjectName%\project"
if errorlevel 1 (
	echo projectフォルダの作成に失敗しました。
	pause
	exit /b 1
)

rem === ステップ3: gitリポジトリを初期化 ===
pushd "%ProjectName%\project"
git init
if errorlevel 1 (
	echo git初期化に失敗しました。
	popd
	pause
	exit /b 1
)

rem === ステップ4: サブモジュールの追加 ===
git submodule add https://github.com/TaiseiHamaya/DirectXGame.git
if errorlevel 1 (
	echo サブモジュールの追加に失敗しました。
	popd
	pause
	exit /b 1
)

rem === ステップ5: CopyFolderRootフォルダの内容をコピー ===
xcopy ".\ProjectGeneratorTool\CopyFolderRoot\*" ".\" /E /H /Y
if errorlevel 1 (
	echo コピーに失敗しました。
	popd
	pause
	exit /b 1
)

rem === ステップ6: __Template__ をプロジェクト名に置換 ===
rem → すべてのファイルに対して__Template__を置換
for /R %%F in (*) do (
	if not "%%~xF"==".exe" (
		powershell -Command "(Get-Content -Raw -Encoding UTF8 '%%F') -replace '__Template__', '%ProjectName%' | Set-Content -Encoding UTF8 '%%F'"
	)
)

popd
echo 完了しました。
pause
