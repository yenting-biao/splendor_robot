#include "splender.h"
#include <bits/stdc++.h>
using namespace std;
#define server 0
#define me 1
#define MAX_DEPTH 7

typedef struct{
	int score = 0;
	int gem[5] = {0, 0, 0, 0, 0};
	int bonus[5] = {0, 0, 0, 0, 0};
	int gold = 0;
	card reserve[3];
	bool reserve_ornot[3] = {false, false, false};
}Player;

typedef struct{
	stack<card> mystack[3];
	card faceup[3][4];
	int gem[5] = {4, 4, 4, 4, 4};
	int goldNum = 4;
}Desk;

Player player[2];
Desk desk;
card empty_card;

void init (vector<card> stack_1, vector<card> stack_2, vector<card> stack_3){
	empty_card.id = -1;

	// set desk variables
	for(int i = stack_1.size() - 1; i >= 0; i--){
		desk.mystack[2].push(stack_1[i]);
	}
	for(int i = stack_2.size() - 1; i >= 0; i--){
		desk.mystack[1].push(stack_2[i]);
	}
	for(int i = stack_3.size() - 1; i >= 0; i--){
		desk.mystack[0].push(stack_3[i]);
	}

	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 4; j++){
			desk.faceup[i][j] = desk.mystack[i].top();
			desk.mystack[i].pop();
		}
	}

	for(int i = 0; i < 5; i++) desk.gem[i] = 4;
	desk.goldNum = 4;

	// set player variables
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 5; j++){
			player[i].gem[j] = 0;
			player[i].bonus[j] = 0;
			if(j < 3) player[i].reserve_ornot[j] = false;
		}
		player[i].score = 0;
		player[i].gold = 0;
	}
}

void removeCard(int a, int b){
	desk.faceup[a][b] = empty_card;
	if(!desk.mystack[a].empty()){
		desk.faceup[a][b] = desk.mystack[a].top();
		desk.mystack[a].pop();
	}
}

void func_removeCard(Desk &d, int a, int b){
	d.faceup[a][b] = empty_card;
	if(!d.mystack[a].empty()){
		d.faceup[a][b] = d.mystack[a].top();
		d.mystack[a].pop();
	}
}

void takeGem(int ID, int bought_gem[5]){
	for(int i = 0; i < 5; i++){
		player[ID].gem[i] += bought_gem[i];
		desk.gem[i] -= bought_gem[i];
	}
}

void func_takeGem(int ID, int bought_gem[5], Player &p, Desk &d){
	for(int i = 0; i < 5; i++){
		p.gem[i] += bought_gem[i];
		d.gem[i] -= bought_gem[i];
	}
}

void buyDeskCard(int ID, int a, int b){
	card toBuy = desk.faceup[a][b];
	for(int i = 0; i < 5; i++){
		if(player[ID].gem[i] + player[ID].bonus[i] >= toBuy.cost[i]){
			int toGiveBackGem = max(toBuy.cost[i] - player[ID].bonus[i], 0);
			desk.gem[i] += toGiveBackGem;
			player[ID].gem[i] -= toGiveBackGem;
		}else{
			int toGiveBackGold = toBuy.cost[i] - player[ID].bonus[i] - player[ID].gem[i];
			desk.gem[i] += player[ID].gem[i];
			player[ID].gem[i] = 0;

			player[ID].gold -=  toGiveBackGold;
			desk.goldNum += toGiveBackGold;
		}
	}
	player[ID].score += toBuy.score;
	player[ID].bonus[toBuy.gem]++;
	removeCard(a, b);
}

void func_buyDeskCard(Player &p, Desk &d, int a, int b){
	card toBuy = d.faceup[a][b];
	for(int i = 0; i < 5; i++){
		if(p.gem[i] + p.bonus[i] >= toBuy.cost[i]){
			int toGiveBackGem = max(toBuy.cost[i] - p.bonus[i], 0);
			d.gem[i] += toGiveBackGem;
			p.gem[i] -= toGiveBackGem;
		}else{
			int toGiveBackGold = toBuy.cost[i] - p.bonus[i] - p.gem[i];
			d.gem[i] += p.gem[i];
			p.gem[i] = 0;

			p.gold -=  toGiveBackGold;
			d.goldNum += toGiveBackGold;
		}
	}
	p.score += toBuy.score;
	p.bonus[toBuy.gem]++;
	func_removeCard(d, a, b);
}

void buyReserveCard(int ID, int a){
	player[ID].reserve_ornot[a] = false;
	card toBuy = player[ID].reserve[a];
	for(int i = 0; i < 5; i++){
		if(player[ID].gem[i] + player[ID].bonus[i] >= toBuy.cost[i]){
			int toGiveBackGem = max(toBuy.cost[i] - player[ID].bonus[i], 0);
			desk.gem[i] += toGiveBackGem;
			player[ID].gem[i] -= toGiveBackGem;
		}else{
			int toGiveBackGold = toBuy.cost[i] - player[ID].bonus[i] - player[ID].gem[i];
			desk.gem[i] += player[ID].gem[i];
			player[ID].gem[i] = 0;

			player[ID].gold -=  toGiveBackGold;
			desk.goldNum += toGiveBackGold;
		}
	}
	player[ID].bonus[toBuy.gem]++;
	player[ID].score += toBuy.score;
	player[ID].reserve[a] = empty_card;
	return;
}

void func_buyReserveCard(Player &p, Desk &d, int a){
	p.reserve_ornot[a] = false;
	card toBuy = p.reserve[a];
	for(int i = 0; i < 5; i++){
		if(p.gem[i] + p.bonus[i] >= toBuy.cost[i]){
			int toGiveBackGem = max(toBuy.cost[i] - p.bonus[i], 0);
			d.gem[i] += toGiveBackGem;
			p.gem[i] -= toGiveBackGem;
		}else{
			int toGiveBackGold = toBuy.cost[i] - p.bonus[i] - p.gem[i];
			d.gem[i] += p.gem[i];
			p.gem[i] = 0;

			p.gold -=  toGiveBackGold;
			d.goldNum += toGiveBackGold;
		}
	}
	p.bonus[toBuy.gem]++;
	p.score += toBuy.score;
	p.reserve[a] = empty_card;
	return;
}

void reserveCard(int ID, int a, int b){
	card toReserve = desk.faceup[a][b];
	for(int i = 0; i < 3; i++){
		if(!player[ID].reserve_ornot[i]){
			player[ID].reserve_ornot[i] = true;
			player[ID].reserve[i] = toReserve;
			player[ID].gold++;
			desk.goldNum--;
			removeCard(a, b);
			return;
		}
	}
}

void func_reserveCard(Player &p, Desk &d, int a, int b){
	card toReserve = d.faceup[a][b];
	for(int i = 0; i < 3; i++){
		if(!p.reserve_ornot[i]){
			p.reserve_ornot[i] = true;
			p.reserve[i] = toReserve;
			p.gold++;
			d.goldNum--;
			func_removeCard(d, a, b);
			return;
		}
	}
}

bool affordableToBuyCard(int ID, card theCard){
	if(theCard.id < 0) return false;
	int notEnough = 0;
	for(int i = 0; i < 5; i++){
		if(player[ID].gem[i] + player[ID].bonus[i] < theCard.cost[i]){
			notEnough += theCard.cost[i] - player[ID].gem[i];
		}
	}
	return (notEnough <= player[ID].gold);
}

bool func_affordableToBuyCard(Player p, card theCard){
	if(theCard.id < 0) return false;
	int notEnough = 0;
	for(int i = 0; i < 5; i++){
		if(p.gem[i] + p.bonus[i] < theCard.cost[i]){
			notEnough += theCard.cost[i] - p.gem[i];
		}
	}
	return (notEnough <= p.gold);
}

int sumGemNum(int ID){
	return player[ID].gem[0] + player[ID].gem[1] + player[ID].gem[2] + player[ID].gem[3] + player[ID].gem[4];
}

int func_sumGemNum(Player p){
	return p.gem[0] + p.gem[1] + p.gem[2] + p.gem[3] + p.gem[4];
}

int sumPlayerReserveNum(Player p){
	int t = 0;
	for(int i = 0; i < 3; i++){
		if(p.reserve_ornot[i]) t++;
	}
	return t;
}

int deskAvailableGemType(bool avail[5]){
	int type = 0;
	for(int i = 0; i < 5; i++){
		if(desk.gem[i] > 0){
			avail[i] = true;
			type++;
		} 
		else avail[i] = false;
	}
	return type;
}

void printDesk(){
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	cout << "faceup ID:\n";
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 4; j++){
			cout << desk.faceup[i][j].id << " ";
		}
		cout << "\n";
	}
	cout << "gem: ";
	for(int i = 0; i < 5; i++){
		cout << desk.gem[i] << " ";
	}
	cout << desk.goldNum << "\n";
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
}

void printPlayer(int ID){
	cout << "********************************************************************************************\n";
	if(ID == 0) cout << "player 1\n";
	else cout << "player 0\n";
	
	cout << "score: " << player[ID].score << "\n";
	cout << "reserve: ";
	for(int i = 0; i < 3; i++){
		if(player[ID].reserve_ornot[i]){
			cout << player[ID].reserve[i].id << " ";
		}
	}
	cout << "\n";
	
	cout << "gem: ";
	for(int i = 0; i < 5; i++){
		cout << player[ID].gem[i] << " ";
	}
	cout << player[ID].gold << "\n";

	cout << "bonus: ";
	for(int i = 0; i < 5; i++){
		cout << player[ID].bonus[i] << " ";
	}
	cout << "\n";

	cout << "********************************************************************************************\n";
}

int minimax_scoreFunc(Desk func_desk, Player func_player[2]){
	int scoreDiff = func_player[me].score - func_player[server].score;
	int bonusDiff = 0;
	int gemDiff = 0;
	for(int i = 0; i < 5; i++){
		bonusDiff += func_player[me].bonus[i] - func_player[server].bonus[i];
		gemDiff += func_player[me].gem[i] - func_player[server].gem[i];
	}
	gemDiff += func_player[me].gold - func_player[server].gold;
	
	int currScore = max(func_player[0].score, func_player[1].score);
	int finalScore = scoreDiff * 10 + bonusDiff * 5 + gemDiff * 4;
	if(currScore < 5){
		finalScore = scoreDiff * 5 + bonusDiff * 8 + gemDiff * 10;
	}else if(currScore < 10){
		finalScore = scoreDiff * 9 + bonusDiff * 8 + gemDiff * 4;
	}else{
		finalScore = scoreDiff * 10 + bonusDiff * 5 + gemDiff * 4;
	}
	return finalScore;
}

int minimax(Desk func_desk, Player func_player[2], int depth, bool maxLevel, struct move &optimal_m, int alpha, int beta){
	if(depth == MAX_DEPTH || func_player[0].score >= 15 || func_player[1].score >= 15){
		return minimax_scoreFunc(func_desk, func_player);
	}

	if(maxLevel){
		int maxScore = INT_MIN;

		/* takeGem for (1, 1, 1) */
		if(func_sumGemNum(func_player[me]) + 3 + func_player[me].gold <= 10){
			for(int i = 0; i < 3; i++){
				for(int j = i + 1; j < 5; j++){
					for(int k = j + 1; k < 5; k++){
						struct move tmpMove;
						tmpMove.type = 1;
						tmpMove.gem[0] = 0; tmpMove.gem[1] = 0; tmpMove.gem[2] = 0; tmpMove.gem[3] = 0; tmpMove.gem[4] = 0;
						tmpMove.gem[i] = 1;
						tmpMove.gem[j] = 1;
						tmpMove.gem[k] = 1;
						if(func_desk.gem[i] >= 1 && func_desk.gem[j] >= 1 && func_desk.gem[k] >= 1){
							Desk tmp_desk = func_desk;
							Player tmp_player[2]; tmp_player[0] = func_player[0]; tmp_player[1] = func_player[1];
							struct move tmp_optimal_m;
							func_takeGem(me, tmpMove.gem, tmp_player[me], tmp_desk);

							int tmpScore = minimax(tmp_desk, tmp_player, depth + 1, false, tmp_optimal_m, alpha, beta);
							if(tmpScore > maxScore){
								maxScore = tmpScore;
								optimal_m = tmpMove;
								alpha = max(alpha, maxScore);
								if(alpha >= beta){
									return maxScore;
								}
							}
							
						}
					}
				}
			}
		}
		

		/* takeGem for (2) */
		if(func_sumGemNum(func_player[me]) + 2 + func_player[me].gold <= 10){
			for(int i = 0; i < 5; i++){
				struct move tmpMove;
				tmpMove.type = 1;
				tmpMove.gem[0] = 0; tmpMove.gem[1] = 0; tmpMove.gem[2] = 0; tmpMove.gem[3] = 0; tmpMove.gem[4] = 0;
				tmpMove.gem[i] = 2;
				if(func_desk.gem[i] >= 2){
					Desk tmp_desk = func_desk;
					Player tmp_player[2]; tmp_player[0] = func_player[0]; tmp_player[1] = func_player[1];
					struct move tmp_optimal_m;
					func_takeGem(me, tmpMove.gem, tmp_player[me], tmp_desk);
					
					int tmpScore = minimax(tmp_desk, tmp_player, depth + 1, false, tmp_optimal_m, alpha, beta);
					if(tmpScore > maxScore){
						maxScore = tmpScore;
						optimal_m = tmpMove;
						alpha = max(alpha, maxScore);
						if(alpha >= beta){
							return maxScore;
						}
					}
				}
			}
		}
		

		/* buy a card on desk */
		for(int i = 0; i < 3; i++){
			for(int j = 0; j < 4; j++){
				card theCard = func_desk.faceup[i][j];
				if(theCard.id == -1) continue;

				struct move tmpMove;
				tmpMove.type = 2;
				tmpMove.card_id = theCard.id;
				if(func_affordableToBuyCard(func_player[me], theCard)){
					Desk tmp_desk = func_desk;
					Player tmp_player[2]; tmp_player[0] = func_player[0]; tmp_player[1] = func_player[1];
					struct move tmp_optimal_m;
					func_buyDeskCard(tmp_player[me], tmp_desk, i, j);

					int tmpScore = minimax(tmp_desk, tmp_player, depth + 1, false, tmp_optimal_m, alpha, beta);
					if(tmpScore > maxScore){
						maxScore = tmpScore;
						optimal_m = tmpMove;
						alpha = max(alpha, maxScore);
						if(alpha >= beta){
							return maxScore;
						}
					}
				}
			}
		}

		/* buy a card that is reserved */
		for(int i = 0; i < 3; i++){
			card theCard = func_player[me].reserve[i];
			if(func_player[me].reserve_ornot[i] && func_affordableToBuyCard(func_player[me], theCard)){
				struct move tmpMove;
				tmpMove.type = 2;
				tmpMove.card_id = theCard.id;

				Desk tmp_desk = func_desk;
				Player tmp_player[2]; tmp_player[0] = func_player[0]; tmp_player[1] = func_player[1];
				struct move tmp_optimal_m;
				func_buyReserveCard(tmp_player[me], tmp_desk, i);

				int tmpScore = minimax(tmp_desk, tmp_player, depth + 1, false, tmp_optimal_m, alpha, beta);
				if(tmpScore > maxScore){
					maxScore = tmpScore;
					optimal_m = tmpMove;
					alpha = max(alpha, maxScore);
					if(alpha >= beta){
						return maxScore;
					}
				}
			}
		}

		/* reserve a card */
		if(sumPlayerReserveNum(func_player[me]) < 3 && func_sumGemNum(func_player[me]) + 1 + func_player[me].gold <= 10){
			for(int i = 0; i < 3; i++){
				for(int j = 0; j < 4; j++){
					card theCard = func_desk.faceup[i][j];
					struct move tmpMove;
					tmpMove.type = 3;
					tmpMove.card_id = theCard.id;

					Desk tmp_desk = func_desk;
					Player tmp_player[2]; tmp_player[0] = func_player[0]; tmp_player[1] = func_player[1];
					struct move tmp_optimal_m;
					func_reserveCard(tmp_player[me], tmp_desk, i, j);

					int tmpScore = minimax(tmp_desk, tmp_player, depth + 1, false, tmp_optimal_m, alpha, beta);
					if(tmpScore > maxScore){
						maxScore = tmpScore;
						optimal_m = tmpMove;
						alpha = max(alpha, maxScore);
						if(alpha >= beta){
							return maxScore;
						}
					}
				}
			}
		}

		// return the max value
		return maxScore;
	}
	else{ // min Level
		int minScore = INT_MAX;

		/* takeGem for (1, 1, 1) */
		if(func_sumGemNum(func_player[server]) + 3 + func_player[server].gold <= 10){
			for(int i = 0; i < 3; i++){
				for(int j = i + 1; j < 5; j++){
					for(int k = j + 1; k < 5; k++){
						struct move tmpMove;
						tmpMove.type = 1;
						tmpMove.gem[0] = 0; tmpMove.gem[1] = 0; tmpMove.gem[2] = 0; tmpMove.gem[3] = 0; tmpMove.gem[4] = 0;
						tmpMove.gem[i] = 1;
						tmpMove.gem[j] = 1;
						tmpMove.gem[k] = 1;
						if(func_desk.gem[i] >= 1 && func_desk.gem[j] >= 1 && func_desk.gem[k] >= 1){
							Desk tmp_desk = func_desk;
							Player tmp_player[2]; tmp_player[0] = func_player[0]; tmp_player[1] = func_player[1];
							struct move tmp_optimal_m;
							func_takeGem(server, tmpMove.gem, tmp_player[server], tmp_desk);

							int tmpScore = minimax(tmp_desk, tmp_player, depth + 1, true, tmp_optimal_m, alpha, beta);
							if(tmpScore < minScore){
								minScore = tmpScore;
								optimal_m = tmpMove;
								beta = min(beta, minScore);
								if(alpha >= beta){
									return minScore;
								}
							}
						}
					}
				}
			}
		}
		

		/* takeGem for (2) */
		if(func_sumGemNum(func_player[server]) + 2 + func_player[server].gold <= 10){
			for(int i = 0; i < 5; i++){
				struct move tmpMove;
				tmpMove.type = 1;
				tmpMove.gem[0] = 0; tmpMove.gem[1] = 0; tmpMove.gem[2] = 0; tmpMove.gem[3] = 0; tmpMove.gem[4] = 0;
				tmpMove.gem[i] = 2;
				if(func_desk.gem[i] >= 2){
					Desk tmp_desk = func_desk;
					Player tmp_player[2]; tmp_player[0] = func_player[0]; tmp_player[1] = func_player[1];
					struct move tmp_optimal_m;
					func_takeGem(server, tmpMove.gem, tmp_player[server], tmp_desk);
					
					int tmpScore = minimax(tmp_desk, tmp_player, depth + 1, true, tmp_optimal_m, alpha, beta);
					if(tmpScore < minScore){
						minScore = tmpScore;
						optimal_m = tmpMove;
						beta = min(beta, minScore);
						if(alpha >= beta){
							return minScore;
						}
					}
				}
			}
		}
		

		/* buy a card on desk */
		for(int i = 0; i < 3; i++){
			for(int j = 0; j < 4; j++){
				card theCard = func_desk.faceup[i][j];
				if(theCard.id == -1) continue;

				struct move tmpMove;
				tmpMove.type = 2;
				tmpMove.card_id = theCard.id;
				if(func_affordableToBuyCard(func_player[server], theCard)){
					Desk tmp_desk = func_desk;
					Player tmp_player[2]; tmp_player[0] = func_player[0]; tmp_player[1] = func_player[1];
					struct move tmp_optimal_m;
					func_buyDeskCard(tmp_player[server], tmp_desk, i, j);

					int tmpScore = minimax(tmp_desk, tmp_player, depth + 1, true, tmp_optimal_m, alpha, beta);
					if(tmpScore < minScore){
						minScore = tmpScore;
						optimal_m = tmpMove;
						beta = min(beta, minScore);
						if(alpha >= beta){
							return minScore;
						}
					}
				}
			}
		}

		/* buy a card that is reserved */
		for(int i = 0; i < 3; i++){
			card theCard = func_player[server].reserve[i];
			if(func_player[server].reserve_ornot[i] && func_affordableToBuyCard(func_player[server], theCard)){
				struct move tmpMove;
				tmpMove.type = 2;
				tmpMove.card_id = theCard.id;

				Desk tmp_desk = func_desk;
				Player tmp_player[2]; tmp_player[0] = func_player[0]; tmp_player[1] = func_player[1];
				struct move tmp_optimal_m;
				func_buyReserveCard(tmp_player[server], tmp_desk, i);

				int tmpScore = minimax(tmp_desk, tmp_player, depth + 1, true, tmp_optimal_m, alpha, beta);
				if(tmpScore < minScore){
					minScore = tmpScore;
					optimal_m = tmpMove;
					beta = min(beta, minScore);
					if(alpha >= beta){
						return minScore;
					}
				}
			}
		}

		/* reserve a card */
		if(sumPlayerReserveNum(func_player[server]) < 3 && func_sumGemNum(func_player[server]) + 1 + func_player[server].gold <= 10){
			for(int i = 0; i < 3; i++){
				for(int j = 0; j < 4; j++){
					card theCard = func_desk.faceup[i][j];
					struct move tmpMove;
					tmpMove.type = 3;
					tmpMove.card_id = theCard.id;

					Desk tmp_desk = func_desk;
					Player tmp_player[2]; tmp_player[0] = func_player[0]; tmp_player[1] = func_player[1];
					struct move tmp_optimal_m;
					func_reserveCard(tmp_player[server], tmp_desk, i, j);

					int tmpScore = minimax(tmp_desk, tmp_player, depth + 1, false, tmp_optimal_m, alpha, beta);
					if(tmpScore < minScore){
						minScore = tmpScore;
						optimal_m = tmpMove;
						beta = min(beta, minScore);
						if(alpha >= beta){
							return minScore;
						}
					}
				}
			}
		}

		// return the max value
		return minScore;
	}
}

struct move player_move (struct move m){
	/*#ifdef SPLENDER_DEBUG
	cout << "after player 0: ";
	printPlayer(me);
	printDesk();
	printPlayer(server);
	#endif*/
	
	if(m.type == 1){
		takeGem(server, m.gem);
	}else if(m.type == 2){
		for(int i = 0; i < 3; i++){
			for(int j = 0; j < 4; j++){
				if(desk.faceup[i][j].id == m.card_id){
					buyDeskCard(server, i, j);
					goto NextStep;
				}
			}
		}
		for(int i = 0; i < 3; i++){
			if(player[server].reserve_ornot[i] && player[server].reserve[i].id == m.card_id){
				buyReserveCard(server, i);
				goto NextStep;
			}
		}
	}else if(m.type == 3){
		for(int i = 0; i < 3; i++){
			for(int j = 0; j < 4; j++){
				if(desk.faceup[i][j].id == m.card_id){
					reserveCard(server, i, j);
					goto NextStep;
				}
			}
		}
	}
	
	NextStep:

	// use minimax
	int maxScore = minimax(desk, player, 0, true, m, INT_MIN, INT_MAX);
	if(m.type == 1){
		takeGem(me, m.gem);
	}else if(m.type == 2){
		for(int i = 0; i < 3; i++){
			for(int j = 0; j < 4; j++){
				if(desk.faceup[i][j].id == m.card_id){
					buyDeskCard(me, i, j);
				}
			}
		}
		for(int i = 0; i < 3; i++){
			if(player[me].reserve_ornot[i] && player[me].reserve[i].id == m.card_id){
				buyReserveCard(me, i);
			}
		}
	}else if(m.type == 3){
		for(int i = 0; i < 3; i++){
			for(int j = 0; j < 4; j++){
				if(desk.faceup[i][j].id == m.card_id){
					reserveCard(me, i, j);
				}
			}
		}
	}
	return m;
	// end of minimax

	/* buy reserved card */
	for(int i = 0; i < 3; i++){
		card theCard = player[me].reserve[i];
		if(player[me].reserve_ornot[i] && affordableToBuyCard(me, theCard)){
			m.type = 2;
			m.card_id = theCard.id;
			buyReserveCard(me, i);
			return m;
		}
	}

	/* buy desk card */
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 4; j++){
			if(affordableToBuyCard(me, desk.faceup[i][j])){
				m.type = 2;
				m.card_id = desk.faceup[i][j].id;
				buyDeskCard(me, i, j);				
				return m;
			}
		}
	}

	srand(5);

	/* can only reserve a card */ /* or even illegal move */
	if(sumGemNum(me) + 3 + player[me].gold > 10){
		int a = rand() % 3;
		int b = rand() % 4;
		while(desk.faceup[a][b].id == -1){
			a = rand() % 3;
			b = rand() % 4;
		}
		
		m.type = 3;
		m.card_id = desk.faceup[a][b].id;
		reserveCard(me, a, b);
		return m;
	}
	
	
	int tmpChoose = rand() % 2;
	if(tmpChoose == 0){
		/* reserve a card */
		int a = rand() % 3;
		int b = rand() % 4;
		while(desk.faceup[a][b].id == -1){
			a = rand() % 3;
			b = rand() % 4;
		}
		
		m.type = 3;
		m.card_id = desk.faceup[a][b].id;
		reserveCard(me, a, b);
		return m;
	}else{
		/* take Gem */
		bool avail[5] = {0};
		int availType = deskAvailableGemType(avail);
		if(availType == 0){ // illegal !!!!!!!!!!!!!!!!!!!!!!
			return m;
		}
		else if(availType < 3){
			int whichTotakeTwo = rand() % 5;
			while(!avail[whichTotakeTwo] || desk.gem[whichTotakeTwo] < 2){
				whichTotakeTwo = rand() % 5;
			}

			m.type = 1;
			for(int i = 0; i < 5; i++){
				if(i == whichTotakeTwo){
					m.gem[i] = 2;
				}else{
					m.gem[i] = 0;
				}
			}
			takeGem(me, m.gem);
			return m;
		}else{
			vector<int> tmp;
			m.type = 1;
			for(int i = 0; i < 5; i++){
				if(avail[i]){
					tmp.push_back(i);
				}
				m.gem[i] = 0;
			}

			for(int i = 0; i < 3; i++){
				int tmpIndex = rand() % (tmp.size());
				int realIndex = tmp[tmpIndex];
				m.gem[realIndex] = 1;
				tmp.erase(tmp.begin() + tmpIndex);
			}
			takeGem(me, m.gem);
			return m;
		}
	}

    return m;
}