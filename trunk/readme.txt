MMDBridge

ビルド方法
・DirectX SDKをインストールしてinclude/libの設定をする.
  プロジェクトファイルには設定していない.
  バージョンは9.0c以降の2005あたりのを適当に使う.
  ビルド時にDirectXのヘッダでエラーが出たらDirectXのヘッダを適当にコメントアウトする.
  他に対処法があるかもしれないが知らない.
・python3.2 (win32)をインストールする.
・libs/mmd/include, libs/mmd/lib にMikuMikuDanceのDataフォルダに入っている, 
  MMDExport.h, MMDExport.libを置く
・Release/ にMikuMikuDance.exeと同階層にあるファイルを全て上書きコピーする.
・Release/ にpython3.dllを置く.
  (python3.dllはPython32をインストールしたディレクトリのDLLsの中にある)
・VC2008 Express Editionでhook.slnを開いてビルドする.
・Debugビルドは成功したところでデバッグできないので,
  構成にそもそも入れていない. 
  対処法があるかもしれないが知らない.
  めんどくさいがReleaseでMessageBoxデバッグしていた.

その他
・Alembic出力を実装しかけたがSimpleAbcViewerで見れる状態までいけたものの,
  houdini,mayaでインポートできるには至らなかったので途中で放置している.
  有効にするにはAlembicを頑張ってビルドして,libs以下に配置し,
  WITH_ALEMBICを指定してビルドする.

免責事項
・MITライセンスに従います.
・詳細はLICENSE.txtをご参照ください.

更新履歴
2012/03/10 - MMDオフ前に杯動画を見ながら - uimac

http://code.google.com/p/mmdbridge/