# how to build
vcpkgに依存パッケージのビルドを任せて簡単にビルドする手順です。
以下、手順。

# cmakeをインストールして、PATHを通す
* [cmake](https://cmake.org/download/)をインストールしてPATHを通してください。
コマンドプロンプトでcmake.exeを実行できる状態にします。

# vcpkgをsetupする

mmdbridgeのビルドが依存しているalembic, pybind11, python3のセットアップができる。

* [vcpkg](https://github.com/Microsoft/vcpkg)をcloneする。
* bootstrap-vcpkg.batを実行してvcpkgをビルドする。

# vcpkgで依存ライブラリをインストールする

``chcp 65001``が必要なのに注意。

```powershell
VCPKG_DIR> chcp 65001
VCPKG_DIR> .\vcpkg.exe install python3:x64-windows-release alembic[hdf5]:x64-windows-release pybind11:x64-windows-release
```

```
VCPKG_DIR
    installed
        x64-windows-release
            bin
                Alembic.dll
                python311.dll
                hdf5.dll
                hdf5_hl.dll
                imath-3_1.dll
                zlib1.dll
                szip.dll
            include
                pybind11
            lib
                Alembic.lib
                python311.lib
                hdf5.lib
                hdf5_hl.lib
                imath-3_1.lib
                zlib.lib
                szip.lib
```

``VCPKG_DIR/installed/x64-windows-release``以下にビルド成果物が格納されるので以降の手順でこれを利用します。
環境変数``VCPKG_DIR``にvcpkgのトップレベルのディレクトリを設定してください。

例。

```
VCPKG_DIR="C:\vcpkg"
```

# DirectX SDKの準備
昔のD3D9が必要なのでSDKをインストールする必要があります。

* https://www.microsoft.com/en-us/download/details.aspx?id=6812

インストールパスを環境変数``DXSDK_DIR``に設定してください。

例。

```
DXSDK_DIR=C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)
```

あとで、mmdbridgeをビルドするときにエラーが出るのですが下記を参考に修正してください。

* https://gist.github.com/t-mat/1540248#d3dx9corehを修正

# MikuMikuDance_x64フォルダの準備
64bit版のMMDをMikuMikuDance_x64フォルダに展開します。
MMDExport.hは、UTF8(bom無し)に変更してください。

```
mmdbridge
    MikuMikuDance_x64
        MikuMikuDance.exe
        Data
            MMDExport.h
            MMDExport.lib
```

# mmdbridgeのビルド
``cmake_vs2017_64.bat``を実行して生成された``build_vs2017_64/mmdbridge.sln``をビルドしてください。``INSTALL``をビルドすると実行に必要なdllとpyをMikuMikuDance_x64にコピーします。

# mmdbridgeのデバッグ実行
ALL_BUILDプロジェクトのプロパティ - デバッグ - コマンド - 参照で``MikuMikuDance_x64/MikuMikuDance.exe``を指定して``F5``実行するとデバッガをアタッチできます。デバッグビルドには、``/Z7``コンパイルオプションでpdbを埋め込んであります。

