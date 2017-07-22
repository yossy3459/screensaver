/******************************************************
*
*  スクリーンセーバーの実行方法
*
******************************************************/

各添付ファイル:
 ソースコード: screenSaver.c, screenSaver2.c
 記述言語: scrnsaveProgram, scrnsaveProgram2
 Makefile: Makefile


授業で紹介した白色と青色のみのバージョンはscrnsaveProgram、
4色すべて使用している物はscrnsaveProgram2となります。


各ファイルの対応: 
 screenSaver.cをコンパイルすると、scrnsaveProgramが実行されます
screenSaver2.cをコンパイルすると、scrnsaveProgram2が実行されます


コンパイル方法:
 make コマンドを実行してください。
 標準状態では "screenSaver.c" が実行されます。
 もしscrnsaveProgram2を確認する場合、
 Makefile内の "file=" の後のパスを"screenSaver2.c"変更してください


実行方法:
 "make s" で実行可能です