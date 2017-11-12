#include <cstdio>
#include <cstdlib>
#include <map>

using namespace std;

bool BetterThanOther(int *firstCount,int *secondCount,bool &secondWin){

	//处理的很巧妙，从12的最高优先级降下来，直到遇到在某一个优先级另一方没有的，则比出结果。向2+2这种类型，不必担心，因为能够
	//进入这个函数，说明他们两的值都相同，而这里能比出结果，必须要求另一方在同一个优先级下没有值。
		for(int i=12;i>=0;--i){          
			if(firstCount[i] && !secondCount[i])
				return true;
			else if(!firstCount[i] && secondCount[i]){
				secondWin=true;
				return false;
			}
		}

	//	return false;        //如果不加这一句的话，如果上面的for循环没有返回值，则这里将会返回一个数值，而这个数值会代表真，

		//这样就会出现这样的结果，当两个都一样的时候，会出现black win. 因为没有最后的这个return,函数将会返回一个数值如（255），则代表真。
		//看下面additional例子
}


int main(){

	map<char,int> cardConversions;
	cardConversions['2']=0;
	cardConversions['3']=1;
	cardConversions['4']=2;
	cardConversions['5']=3;
	cardConversions['6']=4;
	cardConversions['7']=5;
	cardConversions['8']=6;
	cardConversions['9']=7;
	cardConversions['T']=8;
	cardConversions['J']=9;
	cardConversions['Q']=10;
	cardConversions['K']=11;
	cardConversions['A']=12;

	int count[2][13];
	int suitPos,cardPos;
	int suitPosStart;
	int start,checking;
	int topTwoMostCommon[2][2];
	bool straight[2],flush[2];
	char line[34];
	char suit;
	while(gets(line)){
		flush[0]=flush[1]=true;
		straight[0]=straight[1]=true;
		suitPos=1;
		cardPos=0;
		topTwoMostCommon[0][0]=topTwoMostCommon[0][1]=topTwoMostCommon[1][0]=topTwoMostCommon[1][1]=-1;

		for(int pos=0;pos<13;pos++)
			count[0][pos]=count[1][pos]=0;

		for(int p=0;p<2;p++){
			suitPosStart=suitPos;
			suit=line[suitPos];
			suitPos += 3;

			for(;suitPos<=suitPosStart+12;suitPos+=3)
				if(line[suitPos] != suit)
					flush[p]=false;

			for(;cardPos<suitPosStart+12;cardPos += 3){
				++count[p][cardConversions[line[cardPos]]];
			}

			start=-1;
			//判断是否straight，然后在找出前两个大小
			for(checking=12;checking>=0;--checking){
				if(count[p][checking]>1)
					straight[p]=false;
				if(count[p][checking]==1 && start==-1)
					start=checking;
				else if(count[p][checking]==0 && (start != -1 && start-checking<=4)){
					straight[p]=false;
				}

				//checking从12减下来，当出现的次数一样的时候，优先保留大值，当出现的时候先保存次数多的
				if(topTwoMostCommon[p][0]==-1 || count[p][topTwoMostCommon[p][0]]<count[p][checking]){  //先考虑缓冲1
					topTwoMostCommon[p][1]=topTwoMostCommon[p][0];
					topTwoMostCommon[p][0]=checking;
				}else if(topTwoMostCommon[p][1]==-1 || count[p][topTwoMostCommon[p][1]]<count[p][checking]){ //先考虑缓冲2
					topTwoMostCommon[p][1]=checking;
				
				}
			}
		}

		//
		int firstMostCommon(topTwoMostCommon[0][0]),firstSecondCommon(topTwoMostCommon[0][1]),secondMostCommon(topTwoMostCommon[1][0]),secondSecondCommon(topTwoMostCommon[1][1]);

		bool firstWin(false),secondWin(false);


		//straight flush
		if(flush[0]&&straight[0]){
			if(flush[1]&&flush[1]){
				if(firstMostCommon>secondMostCommon)
					firstWin=true;
				else if(firstMostCommon<secondMostCommon)
					secondWin=true;
				
			}else{
				firstWin=true;
			}
		}else if(flush[1]&& straight[1])
			secondWin=true;

		//Four of a kind,如果两个人都是4个一样的，绝对可以比出大小，因为没有相同的牌出现8次
		else if(count[0][firstMostCommon]==4){
			if(count[1][secondMostCommon]==4){
				if(firstMostCommon>secondMostCommon)
					firstWin=true;
				else if(firstMostCommon<secondMostCommon)
					secondWin=true;
			}else{
				firstWin=true;
			}
		}
		else if(count[1][secondMostCommon]==4)
			secondWin=true;

		//Full House
		else if(count[0][firstMostCommon]==3 && count[0][firstSecondCommon]==2){
			if(count[1][secondMostCommon]==3 && count[1][secondSecondCommon]==2){
				if(firstMostCommon>topTwoMostCommon[1][0] || (firstMostCommon==secondMostCommon && firstSecondCommon > secondSecondCommon))
					firstWin=true;
				else if(firstMostCommon<topTwoMostCommon[1][0] || (firstMostCommon==secondMostCommon && firstSecondCommon < secondSecondCommon))
					secondWin=true;
			}else
				firstWin=true;
		
		}else if(count[1][secondMostCommon]==3 && count[1][secondSecondCommon]==2)
			secondWin=true;

		//Flush
		else if(flush[0]){
			if(flush[1]){
				firstWin=BetterThanOther(count[0],count[1],secondWin);
			}else
				firstWin=true;
		
		}
		else if(flush[1])
			secondWin=true;

		//Straight
		else if(straight[0]){
			if(straight[1]){
				if(firstMostCommon>secondMostCommon)
					firstWin=true;
				else if(firstMostCommon<secondMostCommon)
					secondWin=true;
			}else
				firstWin=true;
		}
		else if(straight[1])
			secondWin=true;

		//Three of a kind，原理和four of kind一样
		else if(count[0][firstMostCommon]==3){
			if(count[1][secondMostCommon]==3){
				if(firstMostCommon>secondMostCommon)
					firstWin=true;
				else if(firstMostCommon<secondMostCommon)
					secondWin=true;
			}else
				firstWin=true;
		}
		else if(count[1][secondMostCommon]==3)
			secondWin=true;

		//Two Pairs
		else if(count[0][firstMostCommon]==2 && count[0][firstSecondCommon]==2){
			if(count[1][secondMostCommon]==2 && count[1][secondSecondCommon]==2){
				if(firstMostCommon>topTwoMostCommon[1][0] || (firstMostCommon==secondMostCommon && firstSecondCommon > secondSecondCommon))
					firstWin=true;
				else if(firstMostCommon<topTwoMostCommon[1][0] || (firstMostCommon==secondMostCommon && firstSecondCommon < secondSecondCommon))
					secondWin=true;
				else     //处理单独剩下的那个数字
					firstWin = BetterThanOther(count[0], count[1], secondWin);    
			}else
				firstWin=true;
		}
		else if(count[1][secondMostCommon]==2 && count[1][secondSecondCommon]==2)
			secondWin=true;

		//Pair
		else if(count[0][firstMostCommon]==2){
			if(count[1][secondMostCommon]==2){
				if (firstMostCommon > topTwoMostCommon[1][0])
                    firstWin = true;
                    
                else if (firstMostCommon < topTwoMostCommon[1][0])
                    secondWin = true;
                    
                else
                    firstWin = BetterThanOther(count[0], count[1], secondWin);
			}else
				firstWin=true;
		}
		else if(count[1][secondMostCommon]==2)
			secondWin=true;

		//High Card
		else
			firstWin = BetterThanOther(count[0], count[1], secondWin);

		if(firstWin)
			printf("Black wins.\n");
		else if(secondWin)
			printf("White wins.\n");
		else
			printf("Tie.\n");
	}
	//system("pause");
	return 0;
}
