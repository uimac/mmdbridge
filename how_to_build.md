# how to build
vcpkgに依存パッケージのビルドを任せてAlembic込みでビルドする手順を作りました。

# 手順

## vcpkgをsetupする
依存ライブラリのソースダウンロードからビルド、bin, lib, includeへの格納を自動実行できる。

* [vcpkg](https://github.com/Microsoft/vcpkg)をcloneする。
* bootstrap-vcpkg.batを実行してvcpkgをビルドする。

## vcpkgで依存ライブラリをインストールする

コマンドラインの例

```powershell
VCPKG_DIR> .\vcpkg.exe install ilmbase:x64-windows pybind11:x64-windows hdf5:x64-windows
```

MSMPIがインストールされていない環境だと途中でエラーになります。

ERROR
```
    Please install the MSMPI redistributable package before trying to install this port.
    The appropriate installer has been downloaded to:
      D:/vcpkg/downloads/MSMpiSetup-8.0.exe
```

メッセージに書いてある ``VCPKG_DIR/downloads/MSMpiSetup-8.0exe``を手動実行してコマンドを再実行すればインストールできました。権限の問題かと思われます。

```
VCPKG_DIR
    installed
        x64-windows
            bin
                hdf5.dll
                python36.dll
                など
            include
                pybind11
            lib
```

``VCPKG_DIR/installed``以下にビルド成果物が格納されるので以降の手順でこれを利用します。
環境変数``VCPKG_DIR``にvcpkgのトップレベルを設定してください。

## Alembic
[Alembic](https://github.com/alembic/alembic/releases)
から``alembic-1.7.1``のソースを入手して以下のような階層になるように解凍する。

```
mmdbridge
    + cmake_vs2017_64_alembic.bat
    + alembic-1.7.1
        + lib
          + Alembic
              + CMakeLists.txt # <- これを修正する
```

vcpkgでビルドしたhdf5に対してリンクがうまくいかず。
ちょっと``alembic-1.7.1/ilb/Alembic/CMakeLists.txt``を修正した。

```cmake
ADD_LIBRARY(Alembic ${LIB_TYPE} ${CXX_FILES}) # 51行目
##############################################################################
# 下記を追加
##############################################################################
IF (USE_HDF5)
    TARGET_LINK_LIBRARIES(Alembic 
        ${HDF5_LIBRARIES}
        )
    ADD_DEFINITIONS(-DH5_BUILT_AS_DYNAMIC_LIB)
ENDIF()
##############################################################################
# ここまで
##############################################################################
```

``cmake_vs2017_64_alembic.bat``を実行してインストールを実行。
``VCPKG_DIR/installed/x64-windows``にAlembicがインストールされる。

