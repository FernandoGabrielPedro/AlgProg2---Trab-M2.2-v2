#include <iostream>
#include <stdlib.h>
#include <time.h>

struct Jogador {
  int posx;
  int posy;
};

struct Bloco {
  bool isPedra = false;
  bool isStart = false;
  bool isEnd = false;
  bool isRunning = false;
  int result = 0;
};

struct Mapa {
  int tamA;
  int tamL;
  Bloco **bloco;
};


void cls() {
  system("clear");
}

char GetChar() {
  char c;
  std::cin >> c;
  return c;
}


void PrintMapaRaw(Mapa mapa, Jogador player) {
  for (int i = 0; i < mapa.tamA; i++) {
    for (int j = 0; j < mapa.tamL; j++) {
      if(mapa.bloco[i][j].isRunning)
        std::cout << "X";
      else if(i == player.posy && j == player.posx)
        std::cout << "O";
      else if (mapa.bloco[i][j].isPedra)
        std::cout << "^";
      else if (mapa.bloco[i][j].isStart)
        std::cout << "0";
      else if (mapa.bloco[i][j].isEnd)
        std::cout << "1";
      else
        std::cout << "-";

      std::cout << " ";
    }
    std::cout << std::endl;
  }
}


bool MapaOk(Mapa mapa, int alt, int lar) {
  Jogador player = {1, 1};
  mapa.bloco[alt][lar].isRunning = true; //Faz com que o bloco que está rodando seja verificado como "já está rodando"
  //Sempre que o bloco retorna algo, o bloco deixa de "estar rodando"
  //Sempre que o bloco retorna algo, esse retorno é salvo para que o bloco não seja verificado mais de uma vez
  
  if(mapa.bloco[alt][lar].isEnd) { //Se o bloco for o final, retorna true
    mapa.bloco[alt][lar].isRunning = false;
    mapa.bloco[alt][lar].result = true;
    return true;
  } else if(mapa.bloco[alt][lar].isPedra) { //Se o bloco for uma pedra retorna false
    mapa.bloco[alt][lar].isRunning = false;
    mapa.bloco[alt][lar].result = false;
    return false;
  } else if(mapa.bloco[alt][lar].result != 0) { //Se o bloco já teve algum retorno, retorna o resultado já obtido
    mapa.bloco[alt][lar].isRunning = false;
    if(mapa.bloco[alt][lar].result == 1)
      return true;
    else
      return false;
  }

  //Tirar o comentário para ver o código funcionando
  /*system("clear");
  PrintMapaRaw(mapa, player);
  std::cin.get();*/

  //Verifica, na ordem, o blocos adjascentes à direita, baixo, esquerda, e depois em cima
  //Se o bloco retornar verdadeiro, retorna verdadeiro
  if(!mapa.bloco[alt][lar+1].isRunning) {
    if(MapaOk(mapa, alt, lar+1)) {
      mapa.bloco[alt][lar].isRunning = false;
      mapa.bloco[alt][lar].result = 1;
      return true;
    }
  }
  if(!mapa.bloco[alt+1][lar].isRunning) {
    if(MapaOk(mapa, alt+1, lar)) {
      mapa.bloco[alt][lar].isRunning = false;
      mapa.bloco[alt][lar].result = 1;
      return true;
    }
  }
  if(!mapa.bloco[alt][lar-1].isRunning) {
    if(MapaOk(mapa, alt, lar-1)) {
      mapa.bloco[alt][lar].isRunning = false;
      mapa.bloco[alt][lar].result = 1;
      return true;
    }
  }
  if(!mapa.bloco[alt-1][lar].isRunning) {
    if(MapaOk(mapa, alt-1, lar)){
      mapa.bloco[alt][lar].isRunning = false;
      mapa.bloco[alt][lar].result = 1;
      return true;
    }
  }

  //Se nenhum dos blocos adjascentes retornar verdadeiro, retorna falso
  mapa.bloco[alt][lar].isRunning = false;
  mapa.bloco[alt][lar].result = 2;
  return false;
}


Mapa CriarMapa(int tamA, int tamL) {
  Mapa newMapa;

  newMapa.tamA = tamA + 2;
  newMapa.tamL = tamL + 2;

  newMapa.bloco = new Bloco *[newMapa.tamA];

  newMapa.bloco[0] = new Bloco[newMapa.tamA * newMapa.tamL];
  for (int i = 1; i < newMapa.tamA; i++) {
    newMapa.bloco[i] = newMapa.bloco[i - 1] + newMapa.tamL;
  }

  for (int i = 0; i < newMapa.tamA; i++) {
    for (int j = 0; j < newMapa.tamL; j++) {
      if (i == 0 || i == newMapa.tamA - 1 || j == 0 || j == newMapa.tamL - 1) { // Se forem as bordas do mapa, serão definidas como pedra e pularão para a próxima posição.
        newMapa.bloco[i][j].isPedra = true;
        continue;
      } else if (i == 1 && j == 1) {
        newMapa.bloco[i][j].isStart = true;
        continue;
      } else if (i == tamA && j == tamL) {
        newMapa.bloco[i][j].isEnd = true;
        continue;
      }

      if ((rand() % 10 + 1) <= 6) { // 20% de chance de a posição normal ser uma pedra
        newMapa.bloco[i][j].isPedra = true;
      }
    }
  }

  return newMapa;
}


Jogador Movimentar(Jogador player, Mapa fase, char c) {
  if (c == 'w' && !fase.bloco[player.posy - 1][player.posx].isPedra)
    player.posy = player.posy - 1;
  if (c == 'a' && !fase.bloco[player.posy][player.posx - 1].isPedra)
    player.posx = player.posx - 1;
  if (c == 's' && !fase.bloco[player.posy + 1][player.posx].isPedra)
    player.posy = player.posy + 1;
  if (c == 'd' && !fase.bloco[player.posy][player.posx + 1].isPedra)
    player.posx = player.posx + 1;

  return player;
}


void JogarFase(Jogador &player, Mapa fase) {
  char input;

  while (true) {
    system("clear");
    PrintMapaRaw(fase, player);
    std::cout << "wasd -> Movimentar\n"
              << "c -> Status\n\n";
    input = GetChar();
    if (input == 'w' || input == 'a' || input == 's' || input == 'd') {
      player = Movimentar(player, fase, input);
    }

    if (fase.bloco[player.posy][player.posx].isEnd)
      return;
  }
}


int main() {
  srand(time(NULL));

  Jogador player;
  Mapa mapa;
  while(true) {
    system("clear");
    player.posx = 1;
    player.posy = 1;
    while(true) {
      mapa = CriarMapa(10, 20);
      if(MapaOk(mapa, 1, 1)) break;
    }
    JogarFase(player, mapa);
  }
}