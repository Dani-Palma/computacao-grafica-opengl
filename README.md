# Computação Gráfica com OpenGL

Projetos da unidade curricular de Computação Gráfica da Licenciatura em Engenharia Informática (Universidade Politécnica de Portalegre, 2024/25), desenvolvidos em C com OpenGL e GLUT.

## Pac-Man (`pacman`)

Versão do jogo clássico de labirinto com mapas carregados a partir de ficheiro (`maps.txt`), movimento do jogador e fantasmas.

## Aplicação de desenho (`paint`)

Editor de desenho 2D para criar linhas, triângulos, quadriláteros, círculos, hexágonos e estrelas, com paleta de cores de fundo e contorno, e para mover, alterar e apagar formas com o rato.

## Como compilar

Requer um compilador C e a biblioteca freeglut.

```bash
gcc pacman/*.c -o pacman -lfreeglut -lopengl32 -lglu32
```

No Windows também pode ser aberto num projeto do Visual Studio com freeglut configurado.

## Tecnologias

C, OpenGL, GLUT

## Autores

- Daniel Palma
- Santiago Molina Sanchéz
