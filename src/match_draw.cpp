#include "match.hpp"

//draw the board after each move
void Match::draw(){
    //Clear screen
    SDL_RenderClear( Basic::instance().m_renderer );

    //Render texture to screen
    board.draw();

    drawSquare(); //draw the squares

    FU(i, 0, 8) FU(j, 0, 8)
        piece[i][j].setVal(chessKind, board.getValue(i, j));

    if(!endFlag){
        if(reCalculate) calculate(), reCalculate = false;
        if(promote != -1) drawPromotion();
        else if(holdPiece) drawDot(); //draw the movable square
    }
    else drawEndgame();

    //draw the pieces
    FU(i, 0, 8) FU(j, 0, 8)
        if(promote == -1 || tempBoard[i][j] == -1) piece[i][j].draw(square[i][j]);

    drawText();
    //important: draw buttons and blur screen
    drawButton();

    //Update screen
    SDL_RenderPresent( Basic::instance().m_renderer );
    SDL_Delay(20);
}

void Match::drawSquare(){
    if(!dMove.empty()){
        addColorSquare(COLOR_MOVED, dMove.back().from);
        addColorSquare(COLOR_MOVED, dMove.back().to);
    }
    if(holdPiece && (dMove.empty() || cur != dMove.back().to)) addColorSquare(COLOR_MOVED, cur);
}

void Match::drawDot(){
    Uint64 temp = movable[cur.fi][cur.se];
    short _ = -1;
    FU(i, 0, 8) FU(j, 0, 8){
        ++ _;
        if(!ONBIT(temp, _)){canMoveTo[i][j] = 0; continue;}
        canMoveTo[i][j] = 1;
        if(piece[i][j].getVal() == -1) dot1.draw(square[i][j]);
        else dot2.draw(square[i][j]);
    }
}

void Match::drawText(){
    Texture o;
    if(endFlag) o = Texture(font, "Match over !!!", COLOR_TEXT_6);
    else{
        string S = "Turn no. "; S += convert(numTurn, -1);
        o = Texture(font, S, COLOR_TEXT_6);
    }
    SDL_Rect R = STATUS_RECT;
    o.draw(R.x + (R.w - o.getW()) /2, R.y + (R.h - o.getH()) /2);
}

void Match::drawButton(){
    bool ok = state == NORMAL_SCR;
    if(Mix_VolumeMusic(-1) != 0) audio.draw(ok);
    else no_audio.draw(ok);
    if(Mix_MasterVolume(-1) != 0) sfx.draw(ok);
    else no_sfx.draw(ok);
    setting.draw(ok);
    change_pieces.draw(ok);

    if(currentSide == BLACK) circleOn.draw(STATUS_RECT.x + 20, STATUS_RECT.y + 15);
    else circleOn.draw(STATUS_RECT.x + 20, STATUS_RECT.y + 255);
    if(!endFlag){
        if(numTurn < 3){
            draw_button.draw(-1);
            resign_button.draw(-1);
        }
        else{
            draw_button.draw(ok);
            resign_button.draw(ok);
        }
    }
    else{
        if(!saveMatch) matchSave_button.draw(ok);
        else matchSaved_button.draw(SAVE_MATCH_RECT);
    }

    if(state == SETTING_SCR){
        Basic::instance().blurScreen();
        settingBox.draw();
    }
    if(state == REPLAY_SCR || state == QUIT_SCR){
        settingBox.draw(0);
        Basic::instance().blurScreen();
        if(state == REPLAY_SCR) replay_prompt.draw();
        else quit_prompt.draw();
    }
    if(state == DRAW_SCR){
        Basic::instance().blurScreen();
        draw_prompt.draw();
    }
    if(state == RESIGN_SCR){
        Basic::instance().blurScreen();
        if(currentSide == WHITE) resignWhite_prompt.draw();
        else resignBlack_prompt.draw();
    }
}

void Match::drawEndgame(){
    assert(endFlag == true);
    FU(i, 0, 8) FU(j, 0, 8) if(piece[i][j].getVal() == KING){
        if(piece[i][j].getSide() == WHITE) WKingSym.draw(smSquare[i][j]);
        else BKingSym.draw(smSquare[i][j]);
    }
}

//add the promotion
void Match::drawPromotion(){
    Texture T;
    FU(i, 0, 8) FU(j, 0, 8) if(tempBoard[i][j] > 0) {
        addColorSquare(COLOR_PROMOTE, make_pair(i, j));
        T = Texture(PieceLink[chessKind][tempBoard[i][j]]);
        T.draw(square[i][j]);
    }
}

//
void Match::create_promotionBoard(int col){
    promote = col;
    int dir = (currentSide == WHITE ? 1 : -1), coef = NUM_OF_PIECES * currentSide,
        row = (currentSide == WHITE ? 0 : 7);
    FU(i, 0, 8) FU(j, 0, 8) tempBoard[i][j] = -1;
    tempBoard[cur.fi][cur.se] = 0;
    for(int i = 0, k = coef + QUEEN; i < 4; ++i, --k){
        int cRow = row + dir*i;
        tempBoard[cRow][col] = k;
    }
}

//draw animation
void Match::drawAnimation_SFX(const Movement& X){
    int Ally = 6 * currentSide, Enemy = 6 - Ally;
    vector<Change> ani;
    bool playEffect = false;

    if(inCheck) checkSFX.play(), playEffect = true;

    if(X.speCase == PROMOTING){
        promoteSFX.play(), playEffect = true;
        ani.push_back(Change(X.from, X.to, -(Ally + X.promoteTo + 1)));
        if(X.pieceTaken != -1) ani.push_back(Change(X.to, Enemy + X.pieceTaken));
        return;
    }
    if(!playEffect && X.pieceTaken != -1) captureSFX.play(), playEffect = true;
    if(X.speCase == NOTHING){
        if(!playEffect) moveSFX.play(), playEffect = true;
        ani.push_back(Change(X.from, X.to, Ally + X.pieceKind));
        if(X.pieceTaken != -1) ani.push_back(Change(X.to, Enemy + X.pieceTaken));
    }
    if(X.speCase == CASTLING){
        assert(X.pieceKind == KING);
        if(!playEffect) castleSFX.play(), playEffect = true;
        ani.push_back(Change(X.from, X.to, Ally + KING));
        int row = X.from.fi;
        if(X.to.se == 2) ani.push_back(Change(make_pair(row, 0), make_pair(row, 3), Ally + ROOK)); //o-o
        else ani.push_back(Change(make_pair(row, 7), make_pair(row, 5), Ally + ROOK)); //o-o-o
    }
    if(X.speCase == EN_PASSANT){
        assert(X.pieceKind == PAWN);
        captureSFX.play(), playEffect = true;
        ani.push_back(Change(X.from, X.to, Ally + PAWN));
        int row = X.from.fi, col = X.to.se;
        ani.push_back(Change(make_pair(row, col), Enemy + PAWN));
    }
    FU(i, 0, 8) FU(j, 0, 8) tempBoard[i][j] = -1;
    for(auto X: ani){
        tempBoard[X.from.fi][X.from.se] = 0;
        if(X.to.fi != -1) tempBoard[X.to.fi][X.to.se] = 0;
    }
    FU(i, 1, NUM_FRAME) drawAnimationStep(i, ani);
}

void Match::drawAnimationStep(int step, const vector<Change>& ani){
    assert(state == NORMAL_SCR);

    //Clear screen
    SDL_RenderClear( Basic::instance().m_renderer );

    //Render texture to screen
    board.draw();

    drawSquare(); //draw the squares

    //draw the pieces
    FU(i, 0, 8) FU(j, 0, 8)
        if(tempBoard[i][j] == -1) piece[i][j].draw(square[i][j]);

    Texture T;
    for(auto X: ani){
        if(X.Case == MOVED){
            int xFrom = square[X.from.fi][X.from.se].x, yFrom = square[X.from.fi][X.from.se].y;
            int xTo = square[X.to.fi][X.to.se].x, yTo = square[X.to.fi][X.to.se].y;
            int _x = xFrom + (xTo - xFrom) / NUM_FRAME * step, _y = yFrom + (yTo - yFrom) / NUM_FRAME * step;
            T = Texture(PieceLink[chessKind][X.pieceIn]);
            T.draw(SDL_Rect {_x, _y, PIECE_SIZE, PIECE_SIZE});
        }
        else{
            T = Texture(PieceLink[chessKind][X.pieceIn]);
            Uint8 a = 16 * (NUM_FRAME - step);
            T.setAlpha(a);
            T.draw(square[X.from.fi][X.from.se]);
        }
    }

    drawText();
    //important: draw buttons and blur screen
    drawButton();

    //Update screen
    SDL_RenderPresent( Basic::instance().m_renderer );
    SDL_Delay(8);
}

//add color to a specific square
void Match::addColorSquare(Uint32 val, pa X){
    Basic::instance().addColor(val, square[X.fi][X.se], val == COLOR_MOVED);
}
