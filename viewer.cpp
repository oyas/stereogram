//viewer.cpp
/*----------------------------------------------------------------------------------*
 * raw画像の簡易表示プログラム。
 *----------------------------------------------------------------------------------*/

#define MARGIN 50	//余白の幅
#define RZURE 3		//立体部のずれ(ピクセル)
#define COLORSIZE 3	//１ピクセル当たりのカラー(byte)

#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

//プロトタイプ宣言
bool getsize(int *size, char *str);

//ヘルプを表示する
void help(){
		printf("stereogramとほぼ同じ使用方法です。\n出力ファイル名は指定しないでください。\n");
}

//メイン関数
int main(int argc, char *argv[])
{
	char *inputfilename;
	int width, height;
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
		printf("ヘルプ : stereogram -h\n");
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

	//表示
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
