#include <windows.h>
#include <scrnsave.h>
#include <gl/gl.h>
#include <GL/glut.h>
#include <gl/glu.h>
#include <stdio.h>
#include <sys/types.h>
#include <process.h>
#include <string.h>

/* my: start */
#define BOARD_SIZE 50
#define BOARD_LINE_NUM 10
#define charLength 1000

typedef struct rotateObject{
  char axis;
  char direction;
}rotateObj;

typedef struct angleObject{
  float x;
  float y;
  float z;
}angleObj;
/* my: end */

#define WIDTH 128 //画像の横のピクセル数
#define HEIGHT 128 //画像の縦のピクセル数

void EnableOpenGL(void);
void DisableOpenGL(HWND);
HDC hDC;
HGLRC hRC;
int wx, wy;
GLubyte * bits;
int finish = 0;
GLfloat red[] = { 1.0, 0.0, 0.0, 1.0 };

void rotateFunc(char * ssd, int *num, angleObj *angle){
  rotateObj tempRotate;

  // read axis
  tempRotate.axis = ssd[*num];
  (*num)++;
  if ( ssd[*num]!=',' ){
      printf("Error: ,\n");
      exit(-1);
  }
  (*num)++;

  // read direction
  tempRotate.direction = ssd[*num];
  (*num)++;
  if ( ssd[*num]!=')' ){
      printf("Error: )\n");
      exit(-1);
  }
  (*num)++;

  // printf("tempRotate.axis     : %c\n",tempRotate.axis);
  // printf("tempRotate.direction: %c\n",tempRotate.direction);

  switch ( tempRotate.axis ){
    case 'x':
      if ( tempRotate.direction=='R' )
        (*angle).x-=5;
      else if ( tempRotate.direction=='L' )
        (*angle).x+=5;
      break;
    case 'y':
      if ( tempRotate.direction=='R' )
        (*angle).y-=5;
      else if ( tempRotate.direction=='L' )
        (*angle).y+=5;
    break;
    case 'z':
      if ( tempRotate.direction=='R' )
        (*angle).z-=5;
      else if ( tempRotate.direction=='L' )
        (*angle).z+=5;
    break;
    default:
      fprintf(stderr, "Invalid character.\n");
      exit(0);
  }
}


char colorFunc(char * ssd, int *num){
  char temp;
  temp = ssd[*num];
  (*num)++;
  if ( ssd[*num]!=')' ){
      printf("Error: )\n");
      exit(-1);
  }
  (*num)++;

  return temp;
}

void display (char * ssd, int *ssdLength)
{
  /* ここに描画部分本体を入れる(画面更新のたびに呼ばれる) */
  // declation
  static int num=0,h,i,j;
  static int count=0,count2=0;
  static char func, color = 'W';
  static angleObj angle = {0.0, 0.0, 0.0};
  static int kankaku = 100 / BOARD_LINE_NUM;

  //  文字の取り出し
  num=num % (*ssdLength);
  func = ssd[num];
  num++;

  if ( ssd[num]!='(' ){
      printf("Error: (\n");
      exit(-1);
  }

  num++;

  // 設計した言語の読み取り
  // 関数
  switch ( func ){
      case 'r':
        rotateFunc(ssd, &num, &angle);
        break;
      case 'c':
        color = colorFunc(ssd, &num);
        break;
  }

  if ( ssd[num]!=';' ){
      printf("Error: ;\n");
      exit(-1);
  }
  num++;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //for ( h=0 ; h<BOARD_LINE_NUM ; h++ ){
    for ( i=0 ; i<BOARD_LINE_NUM ; i++ ){
      for ( j=0 ; j<BOARD_LINE_NUM ; j++ ){
        float half_kankaku = (float)kankaku / 2;
        float tempx = (float)kankaku*((BOARD_LINE_NUM/2)-i) - half_kankaku;
        float tempy = (float)kankaku*((BOARD_LINE_NUM/2)-j) - half_kankaku;
        glPushMatrix();

        glTranslatef(-tempx, tempy, 0);
        glRotated(angle.x,1,0,0);//X軸回転
        glRotated(angle.y,0,1,0);//Y軸回転
        glRotated(angle.z,0,0,1);//Y軸回転
        glTranslatef(tempx, -tempy, 0);

        switch ( color ){
          case 'W':
            glColor3d(1,1,1);
            break;
          case 'R':
            glColor3d(1,0,0);
            break;
          case 'G':
            glColor3d(0,1,0);
            break;
          case 'B':
            glColor3d(0,0,1);
            break;
          default: // WRGB以外の文字が含まれていたら終了
            fprintf(stderr, "Invalid character.\n");
            exit(0);
        }

        glBegin(GL_POLYGON);
        glVertex2d(-BOARD_SIZE + kankaku * (i+1), BOARD_SIZE - kankaku * (j+1));
        glVertex2d(-BOARD_SIZE + kankaku * i , BOARD_SIZE - kankaku * (j+1));
        glVertex2d(-BOARD_SIZE + kankaku * i , BOARD_SIZE - kankaku * j);
        glVertex2d(-BOARD_SIZE + kankaku * (i+1), BOARD_SIZE - kankaku * j);
        glEnd();
        glPopMatrix();
      }
    }
  //}

  if ( func == 'r' )
    Sleep(15); // 0.015秒待つ
}

unsigned __stdcall disp (void *arg) {
  char ssd[charLength];
  FILE *fp=NULL;
  int argc, temp, i=0;
  char * argv[2];
  GLuint textureID;

  EnableOpenGL(); // OpenGL設定

  // glut初期化
  argc=1;
  argv[0]="sample2-mt.scr";
  argv[1]=NULL;
  glutInit(&argc, argv);

  /* OpenGL初期設定 */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-50.0*wx/wy, 50.0*wx/wy, -50.0, 50.0, -50.0, 50.0); // 座標系設定

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // glClearするときの色設定
  glColor3f(0.9, 0.8, 0.7); // glRectf等で描画するときの色設定
  glViewport(0,0,wx,wy);
  /* ここまで */

  /* スクリーンセーバ記述プログラム読み込み */
  fp=fopen ("scrnsaveProgram2", "r");
  if (fp==NULL) {
    fprintf (stderr, "failed to open the file \"scrnsaveProgram2\"\n");
    exit(0);
  }

  while ( fscanf (fp, "%c", &ssd[i]) != EOF ){
    i++;
    if ( i==charLength )
      break;
  }
    i--;

  if ( i>charLength ){
    fprintf (stderr, "The length of the input is more than charLength(%d).\n",charLength);
    exit(0);
  }
  else if ( i<0 ){
    fprintf (stderr, "There is not a character in the file \"controler\".\n");
    exit(0);
  }

  while (1){
    if ( ssd[i] == ';' )
      break;
    i--;
  }
  i++;

  fclose(fp);
  /* ここまで */



  /* 表示関数呼び出しの無限ループ */
  while(1) {
    display(ssd, &i); // 描画関数呼び出し
    glFlush(); // 画面描画強制
    SwapBuffers(hDC); // front bufferとback bufferの入れ替え
    if (finish == 1) // finishが1なら描画スレッドを終了させる
      return 0;
  }
}

LRESULT WINAPI ScreenSaverProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  static HANDLE thread_id1;
  static unsigned dummy;
  RECT rc;

  switch(msg) {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    GetClientRect(hWnd, &rc);
    wx = rc.right - rc.left;
    wy = rc.bottom - rc.top;

    thread_id1=(HANDLE)_beginthreadex(NULL, 0, disp, &dummy, 0, &dummy); // 描画用スレッド生成
    if(thread_id1==0){
      fprintf(stderr,"pthread_create : %s", strerror(errno));
      exit(0);
    }
    break;
  case WM_ERASEBKGND:
    return TRUE;
  case WM_DESTROY:
    finish=1; // 描画スレッドを終了させるために1を代入
    WaitForSingleObject(thread_id1, INFINITE); // 描画スレッドの終了を待つ
    CloseHandle(thread_id1);
    DisableOpenGL(hWnd);
    PostQuitMessage(0);
    return 0;
  default:
    break;
  }
  return DefScreenSaverProc(hWnd, msg, wParam, lParam);
}

BOOL WINAPI ScreenSaverConfigureDialog(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
  return TRUE;
}

BOOL WINAPI RegisterDialogClasses(HANDLE hInst)
{
  return TRUE;
}


void EnableOpenGL(void) {
  int format;
  PIXELFORMATDESCRIPTOR pfd = {
    sizeof(PIXELFORMATDESCRIPTOR), // size of this pfd
    1,                     // version number
    0
    | PFD_DRAW_TO_WINDOW   // support window
    | PFD_SUPPORT_OPENGL   // support OpenGL
    | PFD_DOUBLEBUFFER     // double buffered
    ,
    PFD_TYPE_RGBA,         // RGBA type
    24,                    // 24-bit color depth
    0, 0, 0, 0, 0, 0,      // color bits ignored
    0,                     // no alpha buffer
    0,                     // shift bit ignored
    0,                     // no accumulation buffer
    0, 0, 0, 0,            // accum bits ignored
    32,                    // 32-bit z-buffer
    0,                     // no stencil buffer
    0,                     // no auxiliary buffer
    PFD_MAIN_PLANE,        // main layer
    0,                     // reserved
    0, 0, 0                // layer masks ignored
  };

  /* OpenGL設定 */
  format = ChoosePixelFormat(hDC, &pfd);
  SetPixelFormat(hDC, format, &pfd);
  hRC = wglCreateContext(hDC);
  wglMakeCurrent(hDC, hRC);
  /* ここまで */
}

void DisableOpenGL(HWND hWnd)
{
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(hRC);
  ReleaseDC(hWnd, hDC);
}
