# how to build
vcpkgに依存パッケージのビルドを任せて簡単にビルドする手順です。
以下、手順。

# vcpkgをsetupする

mmdbridgeのビルドが依存しているalembic, pybind11のセットアップができる。

* [vcpkg](https://github.com/Microsoft/vcpkg)をcloneする。
* bootstrap-vcpkg.batを実行してvcpkgをビルドする。

# vcpkgで依存ライブラリをインストールする

``chcp 65001``が必要なのに注意。

```powershell
VCPKG_DIR> chcp 65001
VCPKG_DIR> .\vcpkg.exe install alembic:x64-windows pybind11:x64-windows
The following packages will be built and installed:
    alembic:x64-windows
  * hdf5:x64-windows
  * ilmbase:x64-windows
  * msmpi:x64-windows
    pybind11:x64-windows
  * python3:x64-windows
  * szip:x64-windows
  * zlib:x64-windows
Additional packages (*) will be installed to complete this operation.
```

MSMPIがインストールされていない環境だとmsmpiビルドの途中でエラーになります。

ERROR
```
    Please install the MSMPI redistributable package before trying to install this port.
    The appropriate installer has been downloaded to:
      D:/vcpkg/downloads/MSMpiSetup-8.0.exe
```

メッセージに書いてある ``VCPKG_DIR/downloads/MSMpiSetup-8.0exe``を手動実行してコマンドを再実行すればインストールできました。権限の問題かと思われます。
ビルドが完了すると以下のようなディレクトリ構成が出力されます。

```
VCPKG_DIR
    installed
        x64-windows
            bin
                alembic.dll
                python36.dll
            include
                pybind11
            lib
```

``VCPKG_DIR/installed/x64-windows``以下にビルド成果物が格納されるので以降の手順でこれを利用します。
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
INSTALLプロジェクトのプロパティ - デバッグ - コマンド - 参照で``MikuMikuDance_x64/MikuMikuDance.exe``を指定して``F5``実行するとデバッガをアタッチできます。デバッグビルドには、``/Z7``コンパイルオプションでpdbを埋め込んであります。

