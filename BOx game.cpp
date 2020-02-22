#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
#define gap cout<<"\n";
#define gapp cout<<"\n\n";
#define memb(ar,sz) memset(ar, false, sz);
#define printAr(ar) for(auto i:ar)cout<<i<<" ";
#define printArn(ar,n) for(int i = 0;i < n; i++)cout<<ar[i]<<" ";
#define takein(ar, s) for(int i =0; i<s; i++)cin>>ar[i];
#define setV(ar ,value, sz) for(int i = 0 ; i<sz ; i++) ar[i] = value ;
#define MAX 10005
#define dicAr ar[1002]
#define lcm(a,b) ((a/__gcd(a,b))*b);
#define toBin(num) bitset<20>(num).to_string();  //change bit if needed
//global var start
int rc = 0, cc = 0;     // max raw count and column count
int edgeCount = 0;
char table[45][45];   /// to store state or full game plot
int trac[102][4];     ///0->up, 1->right, 2->down, 3->left (track of edge entered on corresponding box)
int anchor[102][2];   ///0->x, 1->y   to store box's corresponding anchor coordinate
map<string, string> holdAnchor;   /// to track anchor coordinates against it's box number
// global var end

string toString(int num)
{
   stringstream st;
   st<<num;
   string s;
   st>>s;
   return s;
}
int toInt(string st)
{
    int a;
   stringstream st1;
   st1<<st;
   st1>>a;
   return a;
}
int updateBoxIndexX(int x)
{
    return x+4;
}
int updateBoxIndexY(int y)
{
    y+=4;
    return (y < cc)? y : 2;
}
void printBoxIndex()                        //not needed, only for test purpose
{
    int boxIndexX = 2, boxIndexY = 2;
    for(int j, i = 0; i<=rc; i++)
    {
        for(j = 0; j<=cc; j++)
        {
            if(boxIndexX == i && boxIndexY == j)
            {
                printf("%d%d ",boxIndexX,boxIndexY);
                boxIndexY = updateBoxIndexY(boxIndexY);
            }
        }
        if(i-1 == boxIndexX) boxIndexX = updateBoxIndexX(boxIndexX);
    }
}
void rander()                         //to visualize final game plot
{
    for(int j, i = 0; i<=rc; i++)
    {
        for(j = 0; j<=cc; j++)
        {
            if(i % 4 == 0)
            {
                if(j % 4 != 0)table[i][j] = '-'; /// all zero and two
            }
            else
            {
                if(j % 4 == 0)table[i][j] = '|'; /// all one and three
            }
        }
    }
}
void printTable()
{
    gap
    for(int j, i = 0; i<=rc; i++)
    {
        printf("  ");
        for(j = 0; j<=cc; j++)printf("%c",table[i][j]);
        gap
    }
}
void setAnchor(int boxNumber, int x, int y)
{
    anchor[boxNumber][0] = x-2;
    anchor[boxNumber][1] = y-2;
}
int BoxNumberOf(int x, int y){
    x = x*10 + y;
    return toInt(holdAnchor[toString(x)]);
}
int giveEdgeTo(int boxNumber, int side, char c)
{
    trac[boxNumber][side] = 1;
    int x = anchor[boxNumber][0],y = anchor[boxNumber][1], sideBox = 0;
    switch(side)
    {
    case 0:
    {
        table[x][y+1] = table[x][y+2] = table[x][y+3] = c;
        if(x != 0)
        {
            x-=4;
            sideBox = BoxNumberOf(x,y);      ///tracking upper box
            trac[sideBox][2] = 1;
        }
        break;
    }
    case 1:
    {
        y+=4;
        table[x+1][y] = table[x+2][y] = table[x+3][y] = c;
        if(y < cc)
        {
            sideBox = BoxNumberOf(x,y);      ///tracking left box
            trac[sideBox][3] = 1;
        }
        break;
    }
    case 2:
    {
        x+=4;
        table[x][y+1] = table[x][y+2] = table[x][y+3] = c;
        if(x < rc)
        {
            sideBox = BoxNumberOf(x,y);      ///tracking lower box
            trac[sideBox][0] = 1;
        }
        break;
    }
    case 3:
    {
        table[x+1][y] = table[x+2][y] = table[x+3][y] = c;
        if(y != 0)
        {
            y-=4;
            sideBox = BoxNumberOf(x,y);      ///tracking right box
            trac[sideBox][1] = 1;
        }
        break;
    }

    }
    return sideBox;
}
void initializePlot()
{
    int boxIndexX = 2, boxIndexY = 2, a; ///a-> to store MSB of boxCount and general holding purpose
    int boxCount = 0;
    for(int j, i = 0; i<=rc; i++)
    {
        j = 0;
        while(j<=cc)
        {
            if(i % 4 == 0)
            {
                if(j % 4 == 0)
                    table[i][j] = 'O';
                else
                {
                    table[i][j] = ' ';            /// all zero and two
                    edgeCount++;
                }
            }
            else if(boxIndexX == i && boxIndexY == j)     //putting box number
            {

                boxIndexY = updateBoxIndexY(boxIndexY);
                setAnchor(++boxCount,i,j);
                a = ((i-2)*10) + (j-2);
                holdAnchor[toString(a)] = toString(boxCount);
                a = boxCount / 10;
                if(a != 0) //num > 9
                {
                    table[i][j] = a + 48;
                    table[i][++j] = (boxCount % 10) + 48;
                }
                else table[i][j] = boxCount + 48;
            }
            else
            {
                if(j % 4 == 0)
                {
                    table[i][j] = ' ';  /// all one and three
                    edgeCount++;
                }
                else table[i][j] = ' ';
            }
            j++;
        }
        if(i-1 == boxIndexX)
            boxIndexX = updateBoxIndexX(boxIndexX);
    }
    edgeCount /= 3;
}

void printAllEnteredEdge()
{
    int sz = (rc * cc)/16;
    gap
    for(int i = 1; i<=sz; i++ )
    {
        printf("For box: %d ->",i);
        if(trac[i][0] == 1)cout<<"up ";
        if(trac[i][1] == 1)cout<<"right ";
        if(trac[i][2] == 1)cout<<"down ";
        if(trac[i][3] == 1)cout<<"left ";
        gap
    }
}
void updatePlot(int boxNumber, char ch)        // to update plot with corresponding player character
{
    int x = anchor[boxNumber][0] + 2, y = anchor[boxNumber][1] + 2;
    table[x][y++] = ch;
    table[x][y] = ' ';
}

bool isGoal(int box, char ch)
{
    if(trac[box][0] == 1 && trac[box][1] == 1 && trac[box][2] == 1 && trac[box][3] == 1)
    {
        updatePlot(box,ch);
        return true;
    }
    return false;

}
int main()
{
    int r = 2, c = 2, userPoint = 0, gameEngine = 0 ;    /// max box capacity 99
    int edg, box, side, sideBox;
    int playerPoint[] = {0,0};
    char ch = '*';
    string st;
    string playerName[2];
    printf("Enter first player name: ");
    cin>>playerName[0];
    printf("Enter second player name: ");
    cin>>playerName[1];
    getchar();
    system("cls");
    printf("%s's character : *\n%s's character : #\n",playerName[0].c_str(),playerName[1].c_str());
    do{
        printf("***Max box capacity 99***\n");
        printf("How many box on column: ");
        scanf("%d",&c);
        printf("How many box on row: ");
        scanf("%d",&r);

    }while(r*c >99);
    rc = r*4;
    cc = c*4;
    initializePlot();
    edg = edgeCount;
    printTable();
    printf("\n.....Game started.....");
    c *= r;                // total box count
    r = 0;                ///reusing variable r for tracking user input

    while(edg > 0)
    {
        gap
        printf("\nNow %s's turn.\n",playerName[r].c_str());
        printf("Enter box number: ");
        scanf("%d",&box);
        printf("0->up, 1->right, 2->down, 3->left\n");
        printf("Enter edge index: ");
        scanf("%d",&side);
        if(side >=0 && side < 4 && box > 0 && box <= c )
        {
            if(trac[box][side] == 1) printf("Already entered");
            else
            {
                system("cls");
                sideBox = (side % 2 == 0) ? giveEdgeTo(box,side,'-') : giveEdgeTo(box,side,'|');
                st = (side == 0)?"upper":((side == 1)?"right":((side == 2)?"down":"left"));
                cout<<"Last entered "<<st<<" side of box "<<box;gap
                if(isGoal(box, ch))playerPoint[r]++;
                if(sideBox != 0 && isGoal(sideBox, ch))playerPoint[r]++;
                printTable();
                printf("\n%s's(*) point:%d   :   %s's(#) point:%d",playerName[0].c_str(),playerPoint[0],playerName[1].c_str(),playerPoint[1]);
                edg--;
                if(r % 2 == 0)
                {
                    ch = '#';
                    ++r;
                }
                else
                {
                    ch = '*';
                    r = 0;
                }
                //printAllEnteredEdge();
            }
        }
        else printf("You just crossed your limit.");
    }
    printf("\nFinally you finished the game.!!\n");
    if(playerPoint[0] > playerPoint[1])printf("Hoho, %s won the game ........\n%s you played well",playerName[0].c_str(),playerName[1].c_str());
    else if(playerPoint[0] < playerPoint[1])printf("Hoho, %s won the game ........\n%s you played well",playerName[1].c_str(), playerName[0].c_str());
    else printf("...Match draw..\n\n\n");
    return 0;
}
