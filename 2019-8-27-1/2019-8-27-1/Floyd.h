#ifndef _FLOYD_H__
#define _FLOYD_H__

#define MAXVEX 20    //最大顶点数
#define INFINITY 65535    //用0表示∞

class Floyd {
public:
  char vers[MAXVEX];    //顶点表
  int way[MAXVEX][MAXVEX];    //方向表
  int arc[MAXVEX][MAXVEX];    //邻接矩阵，可看作边表
  int numVertexes, numEdges;    //图中当前的顶点数和边数
  int Dist[MAXVEX][MAXVEX];    //存放各个顶点到其余顶点的最短路径权值和
  int Path[MAXVEX][MAXVEX];    //存放各个顶点到其余顶点前驱顶点位置
  int heart;                //存放小车当前车头朝向
  void CreateMGraph();//创建邻接矩阵
  void Create_Floyd();//创建佛罗伦萨最短路径矩阵
  void FindWays(int a, int b);//查找从a到b的最短路径

  int numNode;//存放生成的最短路径的节点个数
  int nowNode;//存放生成的最短路径的当前所在的节点位置(whichNode数组中的位置)
  int WhichNode[MAXVEX];//存放生成的最短路径中每个节点的名称
  int WhereNode[MAXVEX];//存放生成的最短路径中每个节点的行驶方向
};
#endif
