#include "Floyd.h"
#define MAXVEX 20    //最大顶点数
#define INFINITY 6553    //用0表示∞
using namespace std;
void Floyd::CreateMGraph()//创建邻接矩阵
{
  int i, j, k;
  numVertexes = 17;
  numEdges = 32;
  //录入顶点信息
  vers[0] = 'A';
  vers[1] = 'B';
  vers[2] = 'C';
  vers[3] = 'D';
  vers[4] = 'E';
  vers[5] = 'F';
  vers[6] = 'G';
  vers[7] = 'H';
  vers[8] = 'I';

  //getchar();    //可以获取回车符
  for (i = 0; i < numVertexes; i++)
    for (j = 0; j < numVertexes; j++)
      arc[i][j] = INFINITY;    //邻接矩阵初始化

      //创建了有向邻接矩阵------------------------
  for (i = 1; i <= 8; i++) {
    arc[0][i] = 10;
  }
  for (i = 1; i <= 8; i++) {
    arc[i][i + 8] = 10;
  }
  for (i = 1; i < 8; i++)
    arc[i][i + 1] = 8;
  arc[1][8] = 7;

  for (i = 9; i < 16; i++)
    arc[i][i + 1] = 15;
  arc[9][16] = 15;
  for (i = 0; i < numVertexes; i++)
    for (k = i; k < numVertexes; k++)
      arc[k][i] = arc[i][k];
  //--------------------------------------------
  //设置图各节点相互前进的方向：
  for (i = 1; i <= 8; i++) {
    way[0][i] = i;
    way[i][0] = (i + 3) % 8 + 1;
  }
  for (i = 1; i <= 8; i++) {
    way[i][i + 8] = i;
    way[i + 8][i] = (i + 3) % 8 + 1;
  }
  for (i = 1; i < 8; i++) {
    way[i][i + 1] = (i + 1) % 8 + 1;
    way[i + 1][i] = (i + 6) % 8 + 1;
  }

  for (i = 9; i < 16; i++) {
    way[i][i + 1] = (i + 1) % 8 + 1;
    way[i + 1][i] = (i + 5) % 8 + 1;
  }
  way[1][8] = 7;
  way[8][1] = 2;
  way[9][16] = 7;
  way[16][9] = 2;

}
void Floyd::Create_Floyd()//创建佛罗伦萨最短路径矩阵
{
  int i, j, k;

  //初始化path和dist
  for (i = 0; i < numVertexes; i++)
  {
    for (j = 0; j < numVertexes; j++)
    {
      Dist[i][j] = arc[i][j];
      Path[i][j] = j;    //初始化为这个的一个好处就是自己到自己的路径就是自己，我们不用修改
    }
  }

  //使用弗洛伊德核心算法，三层循环求解
  for (k = 0; k < numVertexes; k++)
  {
    for (i = 0; i < numVertexes; i++)
    {
      for (j = 0; j < numVertexes; j++)
      {
        if (Dist[i][j] > (Dist[i][k] + Dist[k][j]) && i != j)
        {
          //将权值和更新，路径也变为中转点
          Dist[i][j] = Dist[i][k] + Dist[k][j];
          Path[i][j] = Path[i][k];
        }
      }
    }
  }
}
void Floyd::FindWays(int a, int b) {
  int nowplace = a, end = b;
  int i, j;
  numNode = 0;//总节点数
  nowNode = 0;//当前节点数
  /*WhichNode[0] = nowplace;
  WhereNode[0] = way[nowplace][Path[nowplace][end]];
  numNode++;*/
  while (nowplace != end) {
    WhereNode[numNode] = way[nowplace][Path[nowplace][end]];//下一个节点的方向
    nowplace = Path[nowplace][end];
    WhichNode[numNode] = nowplace;//下一个节点的位置
    numNode++;
  }

}
