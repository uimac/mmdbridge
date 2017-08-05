# MMDBridge

[ビルド方法変わりました](how_to_build.md)

# 元のビルド方法

DirectX SDKをインストールしてinclude/libの設定をする.
  プロジェクトファイルには設定していない.
  バージョンはApril 2006を使用している.
  ビルド時にDirectXのヘッダでエラーが出たらDirectXのヘッダを適当にコメントアウトする.
  他に対処法があるかもしれないが知らない.
・libs/mmd/include, libs/mmd/Win32, にMikuMikuDanceのDataフォルダに入っている, 
  MMDExport.h, MMDExport.libを置く

・Release/Win32/ にMikuMikuDance.exeと同階層にあるファイルを全て上書きコピーする.
・boost.pythonをビルドしてlibs/boostに同じように配置する.
・Python3.4をビルドしてlibs/pythonに同じように配置する.
・(Alembic使用する場合)HDF5をビルドしてlibs/hdf5に同じように配置する.
・(Alembic使用する場合)ilmbaseをビルドしてlibs/ilmbaseに同じように配置する.
・(Alembic使用する場合)Alembicをビルドしてlibs/alembicに同じように配置する.
・Release/Win32/ にビルドしたpython34.dllとPythonのLibフォルダを置く.

・VC2013でhook.slnを開いてビルドする.
・Debugビルドは成功したところでデバッグできないので,
  構成にそもそも入れていない. 
  対処法があるかもしれないが知らない.
  めんどくさいがReleaseでMessageBoxデバッグしていた.

# その他
* Alembicを無効にするにはプリプロセッサの定義のWITH_ALEMBICを外す。
* Alembic含めて完全なビルドをする場合の難易度は, かなり高いです。

免責事項
* MITライセンスに従います.
* 詳細はLICENSE.txtをご参照ください.

# README更新履歴

* 2017/08/06 - ビルドシステムの変更について
* 2015/03/30 - いい加減古くなっていたので更新
* 2012/03/10

http://code.google.com/p/mmdbridge/

