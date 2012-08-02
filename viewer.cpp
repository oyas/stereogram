//rittaishi.cpp
/*----------------------------------------------------------------------------------*
 * 立体視画像を作成するプログラム
 * GUIはないので注意。コマンドとして使う。
 * 書式
 * rittaishi サイズ 入力ファイル 出力ファイル
 * rittaishi -h
 *----------------------------------------------------------------------------------*/

#define MARGIN 50	//余白の幅
#define RZURE 3		//立体部のずれ(ピクセル)
#define COLORSIZE 1	//１ピクセル当たりのカラー(byte)

#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

//プロトタイプ宣言
bool getsize(int *size, char *str);

//ヘルプを表示する
void help(){
		printf("rittaishiとほぼ同じ使用方法です。\n出力ファイル名は指定しないでください。\n");
}

//メイン関数
int main(int argc, char *argv[])
{
	char *str, *inputfilename, *outputfilename, *check;
	int width, height, *size;
	FILE *fp;
		
	//乱数初期化
	srand( time(NULL) );

	//引数チェック
	if(argc != 3){	//引数がおかしいとき
		if( argc == 2 && !strcmp(argv[1], "-h") ){
			help();	//ヘルプ表示
			return 0;
		}
		printf("エラー：引数が少ない、または多いです。\n");	//エラー表示
		printf("ヘルプ : rittaishi -h\n");
		return 1;	//エラー終了1
	}
	//サイズ取得
	if( getsize(&width, strtok(argv[1], "x")) ||	//文字列をxで区切る
		getsize(&height, strtok(NULL, "x")) ){
			//どちらかでエラーが発生した場合
			printf("サイズ取得でエラーを起こしました。1番目の引数を確認してください。");
			return 2;	//エラー終了2
	}
	//入力ファイル名取得
	inputfilename = argv[2];
	//出力ファイル名取得
//	outputfilename = argv[3];

	printf("読み取り結果：\nサイズ　横%d 縦%d\n入力ファイル名%s\n", width, height, inputfilename);

	//入力画像読み込み
	int in_size = width*height*COLORSIZE;
	unsigned char *source = new unsigned char[ in_size ];	//バッファ作成
	if ((fp = fopen(inputfilename, "rb")) != NULL) {
		fread(source, in_size, 1, fp);
		fclose(fp);
	}
	else {
		printf("%sが開けませんでした。", inputfilename);
		return 3;	//エラー終了3
	}
	//printf("sizeof source : %d\n", sizeof source);
/*	
	//画像用配列作成
	const int maxwidth = width + MARGIN * 3;	//作成される画像の幅
	const int maxheight = height + MARGIN * 2;	//作成される画像の高さ
	int out_size = maxwidth * maxheight * 3;
	unsigned char *output = new unsigned char[ out_size ];
	for( int i=0; i < maxwidth * maxheight * 3; i++ ){
		output[i] = 0;	//初期化
	}


	//背景作成
	const int offset = MARGIN * maxwidth + MARGIN;	//左画像の左上位置
	const int pairpos = width + MARGIN;		//隣の対応する点までの距離
	int pos;	//操作点の位置
	for(int y=0; y < height; y++ ){
		for(int x=0; x < width; x++ ){
			pos = offset + y * maxwidth + MARGIN + x;	//操作点
			output[ pos * 3   ] = output[ (pairpos + pos) * 3   ] = (char)(rand()%256); //赤
			output[ pos * 3 +1] = output[ (pairpos + pos) * 3 +1] = (char)(rand()%256); //緑
			output[ pos * 3 +2] = output[ (pairpos + pos) * 3 +2] = (char)(rand()%256); //青
		}
	}
*/	
	//浮き出るものの作成
	for(int y=3; y < height; y+=5 ){
		for(int x=3; x < width; x+=5 ){
			//判定	
			if( source[ (y * width + x)*COLORSIZE ] == 0 ){
				putchar('*');	
			}else{
				putchar(' ');
			}
		}
		putchar('\n');
	}
	
/*
	//ファイルに書き出し
	if( fp = fopen( outputfilename, "wb" ) ){
		fwrite(output, out_size, 1, fp);
		fclose(fp);
	}else{
		printf("%sを作成または開くことができませんでした。", outputfilename);
		return 4;	//エラー終了4
	}
	

	//終了通知
	printf("%dx%dの画像を作成しました。\n", maxwidth, maxheight);	

	//後処理
	delete [] source;
	delete [] output;
*/
	return 0;
}

//サイズ取得に使用
bool getsize(int *size, char *str){
	char *check;
	
	//文字列がNULLならエラー
	if( str == NULL ) return true;
	
	//文字列を整数に変換
	*size = strtol(str, &check, 10);
	if ( errno == ERANGE ){
		perror( "長整数の範囲を超えました。" );
		return true;
	}
	else if ( *check != '\0' ){
		perror( "数字文字列ではありません。" );
		return true;
	}
	if( *size >= 10000 ){
		perror( "デカすぎでしょう。" );
		return true;
	}
	if( *size <= 0 ){
		perror( "サイズが0またはマイナスです。" );
		return true;
	}
	
	return false;
}