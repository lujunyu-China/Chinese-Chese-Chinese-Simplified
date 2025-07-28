#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <windows.h>

using namespace std;

void SetConsoleUTF8() {
	SetConsoleOutputCP(65001);
}

enum PieceType {
	EMPTY,
	GENERAL,
	ADVISOR,
	ELEPHANT,
	HORSE,
	CHARIOT,
	CANNON,
	SOLDIER
};

enum PieceColor {
	NONE,
	RED,
	BLACK
};

struct Piece {
	PieceType type;
	PieceColor color;
	wstring symbol;
};

class ChessBoard {
private:
	vector<vector<Piece>> board;
	bool redTurn;
	bool gameOver;
	
public:
	ChessBoard() : redTurn(true), gameOver(false) {
		board.resize(10, vector<Piece>(9, {EMPTY, NONE, L"　"}));
		initializeBoard();
	}
	
	void initializeBoard() {
		board[9][0] = {CHARIOT, RED, L"俥"};
		board[9][1] = {HORSE, RED, L"傌"};
		board[9][2] = {ELEPHANT, RED, L"相"};
		board[9][3] = {ADVISOR, RED, L"仕"};
		board[9][4] = {GENERAL, RED, L"帅"};
		board[9][5] = {ADVISOR, RED, L"仕"};
		board[9][6] = {ELEPHANT, RED, L"相"};
		board[9][7] = {HORSE, RED, L"傌"};
		board[9][8] = {CHARIOT, RED, L"俥"};
		
		board[7][1] = {CANNON, RED, L"炮"};
		board[7][7] = {CANNON, RED, L"炮"};
		
		for(int i=0;i<5;++i){
			board[6][i*2]={SOLDIER,RED,L"兵"};
		}
		
		board[0][0]={CHARIOT,BLACK,L"車"};
		board[0][1]={HORSE,BLACK,L"馬"};
		board[0][2]={ELEPHANT,BLACK,L"象"};
		board[0][3]={ADVISOR,BLACK,L"士"};
		board[0][4]={GENERAL,BLACK,L"将"};
		board[0][5]={ADVISOR,BLACK,L"士"};
		board[0][6]={ELEPHANT,BLACK,L"象"};
		board[0][7]={HORSE,BLACK,L"馬"};
		board[0][8]={CHARIOT,BLACK,L"車"};
		
		board[2][1]={CANNON,BLACK,L"砲"};
		board[2][7]={CANNON,BLACK,L"砲"};
		
		for(int i=0;i<5;++i){
			board[3][i*2]={SOLDIER,BLACK,L"卒"};
		}
	}
	
	void printBoard(){
		wcout<<L"  ";
		for(int j=0;j<9;++j){
			wcout<<j<<L" ";
		}
		wcout<<endl;
		
		for(int i=0;i<10;++i){
			wcout<<i<<L" ";
			for(int j=0;j<9;++j){
				wcout<<board[i][j].symbol;
			}
			wcout<<endl;
		}
		wcout<<(redTurn?L"红方":L"黑方")<<L"回合"<<endl;
	}
	
	bool isValidMove(int fromX,int fromY,int toX,int toY){
		if(fromX<0||fromX>=10||fromY<0||fromY>=9||
			toX<0||toX>=10||toY<0||toY>=9){
			wcout<<L"移动超出棋盘范围！"<<endl;
			return false;
		}
		
		Piece fromPiece=board[fromX][fromY];
		Piece toPiece=board[toX][toY];
		
		if(fromPiece.type==EMPTY){
			wcout<<L"没有选中棋子！"<<endl;
			return false;
		}
		
		if((redTurn&&fromPiece.color!=RED)||(!redTurn&&fromPiece.color!=BLACK)){
			wcout<<L"请选择自己的棋子！"<<endl;
			return false;
		}
		
		if(toPiece.color==fromPiece.color){
			wcout<<L"不能吃自己的棋子！"<<endl;
			return false;
		}
		
		switch(fromPiece.type){
			case GENERAL:
				if(redTurn){
					if(toX<7||toX>9||toY<3||toY>5){
						wcout<<L"将/帅只能在九宫格内移动！"<<endl;
						return false;
					}
				}else{
					if(toX<0||toX>2||toY<3||toY>5){
						wcout<<L"将/帅只能在九宫格内移动！"<<endl;
						return false;
					}
				}
				
				if(abs(fromX-toX)+abs(fromY-toY)!=1){
					if(fromY==toY){
						bool hasPieceBetween=false;
						int start=min(fromX,toX)+1;
						int end=max(fromX,toX)-1;
						for(int i=start;i<=end;++i){
							if(board[i][fromY].type!=EMPTY){
								hasPieceBetween=true;
								break;
							}
						}
						if(!hasPieceBetween&&board[toX][toY].type==GENERAL){
							return true;
						}
					}
					wcout<<L"将/帅每次只能移动一步！"<<endl;
					return false;
				}
				break;
				
			case ADVISOR:
				if(redTurn){
					if(toX<7||toX>9||toY<3||toY>5){
						wcout<<L"士只能在九宫格内移动！"<<endl;
						return false;
					}
				}else{
					if(toX<0||toX>2||toY<3||toY>5){
						wcout<<L"士只能在九宫格内移动！"<<endl;
						return false;
					}
				}
				
				if(abs(fromX-toX)!=1||abs(fromY-toY)!=1){
					wcout<<L"士只能斜走一步！"<<endl;
					return false;
				}
				break;
				
			case ELEPHANT:
				if(abs(fromX-toX)!=2||abs(fromY-toY)!=2){
					wcout<<L"象必须走田字！"<<endl;
					return false;
				}
				
				int eyeX=(fromX+toX)/2;
				int eyeY=(fromY+toY)/2;
				if(board[eyeX][eyeY].type!=EMPTY){
					wcout<<L"象眼被堵住了！"<<endl;
					return false;
				}
				
				if(redTurn&&toX<5){
					wcout<<L"象不能过河！"<<endl;
					return false;
				}
				if(!redTurn&&toX>4){
					wcout<<L"象不能过河！"<<endl;
					return false;
				}
				break;
				
			case HORSE:
				if(!((abs(fromX-toX)==2&&abs(fromY-toY)==1)&&
					!((abs(fromX-toX)==1&&abs(fromY-toY)==2))){
					wcout<<L"马必须走日字！"<<endl;
					return false;
				}
				
				if(abs(fromX-toX)==2){
					int legX=(fromX+toX)/2;
					if(board[legX][fromY].type!=EMPTY){
						wcout<<L"马腿被别住了！"<<endl;
						return false;
					}
				}else{
					int legY=(fromY+toY)/2;
					if(board[fromX][legY].type!=EMPTY){
						wcout<<L"马腿被别住了！"<<endl;
						return false;
					}
				}
				break;
				
			case CHARIOT:
				if(fromX!=toX&&fromY!=toY){
					wcout<<L"车必须走直线！"<<endl;
					return false;
				}
				
				if(fromX==toX){
					int start=min(fromY,toY)+1;
					int end=max(fromY,toY)-1;
					for(int y=start;y<=end;++y){
						if(board[fromX][y].type!=EMPTY){
							wcout<<L"车移动路径上有阻挡！"<<endl;
							return false;
						}
					}
				}else{
					int start=min(fromX,toX)+1;
					int end=max(fromX,toX)-1;
					for(int x=start;x<=end;++x){
						if(board[x][fromY].type!=EMPTY){
							wcout<<L"车移动路径上有阻挡！"<<endl;
							return false;
						}
					}
				}
				break;
				
			case CANNON:
				if(fromX!=toX&&fromY!=toY){
					wcout<<L"炮必须走直线！"<<endl;
					return false;
				}
				
				int pieceCount=0;
				if(fromX==toX){
					int start=min(fromY,toY)+1;
					int end=max(fromY,toY)-1;
					for(int y=start;y<=end;++y){
						if(board[fromX][y].type!=EMPTY){
							pieceCount++;
						}
					}
				}else{
					int start=min(fromX,toX)+1;
					int end=max(fromX,toX)-1;
					for(int x=start;x<=end;++x){
						if(board[x][fromY].type!=EMPTY){
							pieceCount++;
						}
					}
				}
				
				if(toPiece.type==EMPTY){
					if(pieceCount!=0){
						wcout<<L"炮移动时中间不能有棋子！"<<endl;
						return false;
					}
				}else{
					if(pieceCount!=1){
						wcout<<L"炮吃子时中间必须有一个棋子！"<<endl;
						return false;
					}
				}
				break;
				
			case SOLDIER:
				if(redTurn){
					if(fromX<=toX){
						wcout<<L"兵不能后退！"<<endl;
						return false;
					}
					if(fromX>4){
						if(fromY!=toY||fromX-toX!=1){
							wcout<<L"未过河的兵只能向前走一步！"<<endl;
							return false;
						}
					}else{
						if(abs(fromX-toX)+abs(fromY-toY)!=1){
							wcout<<L"兵只能向前或左右移动一步！"<<endl;
							return false;
						}
					}
				}else{
					if(fromX>=toX){
						wcout<<L"卒不能后退！"<<endl;
						return false;
					}
					if(fromX<5){
						if(fromY!=toY||toX-fromX!=1){
							wcout<<L"未过河的卒只能向前走一步！"<<endl;
							return false;
						}
					}else{
						if(abs(fromX-toX)+abs(fromY-toY)!=1){
							wcout<<L"卒只能向前或左右移动一步！"<<endl;
							return false;
						}
					}
				}
				break;
				
			default:
				return false;
		}
		
		return true;
	}
	
	void movePiece(int fromX,int fromY,int toX,int toY){
		if(!isValidMove(fromX,fromY,toX,toY)){
			return;
		}
		
		if(board[toX][toY].type==GENERAL){
			gameOver=true;
			wcout<<(redTurn?L"红方":L"黑方")<<L"获胜！"<<endl;
		}
		
		board[toX][toY]=board[fromX][fromY];
		board[fromX][fromY]={EMPTY,NONE,L"　"};
		
		redTurn=!redTurn;
	}
	
	bool isGameOver()const{
		return gameOver;
	}
	
	bool isRedTurn()const{
		return redTurn;
	}
};

int main(){
	SetConsoleUTF8();
	
	ChessBoard game;
	
	wcout<<L"中国象棋游戏"<<endl;
	wcout<<L"输入格式：起始行 起始列 目标行 目标列（例如：9 0 8 0）"<<endl;
	wcout<<L"输入'q'退出游戏"<<endl;
	
	while(!game.isGameOver()){
		game.printBoard();
		
		wstring input;
		wcout<<L"请输入你的走法：";
		wcin>>input;
		
		if(input==L"q"||input==L"Q"){
			wcout<<L"游戏结束！"<<endl;
			break;
		}
		
		int fromX,fromY,toX,toY;
		try{
			fromX=stoi(input);
			wcin>>fromY>>toX>>toY;
			
			game.movePiece(fromX,fromY,toX,toY);
		}catch(...){
			wcout<<L"输入无效，请重新输入！"<<endl;
			wcin.clear();
			wcin.ignore(numeric_limits<streamsize>::max(),L'\n');
		}
	}
	
	return 0;
}
