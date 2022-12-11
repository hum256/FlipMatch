# include <Siv3D.hpp> // OpenSiv3D v0.6.6

const int SIZE = 5;
const int WINDOW_WIDTH = ((SIZE+1)*2+1);
const int WINDOW_HEIGHT = (SIZE+4);
Grid<int> map(SIZE, SIZE);
Grid<int> retry(SIZE, SIZE);
Grid<int> start(WINDOW_WIDTH, WINDOW_HEIGHT);



class GameBoard
{
public:
    int diffMapX = 32 * ((WINDOW_WIDTH - 1) / 2 + 1);
    int diffMapY = 32 * (WINDOW_HEIGHT - SIZE) / 2;
    int diffAnsX = 32 * ((WINDOW_WIDTH - 1) / 2 - SIZE);
    int diffAnsY = 32 * (WINDOW_HEIGHT - SIZE) / 2;

    int flip_num = 0;
    int ans_flag = 0;

    Array<Grid<int>> ans = {
        {
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
        },
        {
            {0, 1, 0, 1, 0},
            {1, 0, 1, 0, 1},
            {0, 1, 0, 1, 0},
            {1, 0, 1, 0, 1},
            {0, 1, 0, 1, 0},
        },
        {
            {1, 1, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 1, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 1, 1, 1, 1},
        }
    };
    
    //
    // 初期化
    //
    void Init()
    {
        flip_num = Random(3, 10);
        ans_flag = Random(2);
        CopyMatrix(ans[ans_flag], map);
        for (int i=0; i<flip_num; i++) {
            Flip(Random(SIZE-1), Random(SIZE-1));
        }
        CopyMatrix(map, retry);

        for (int i=0; i<WINDOW_WIDTH; i++) {
            for (int e=0; e<WINDOW_HEIGHT; e++) {
                if (i>=diffAnsX/32 && i<=diffAnsX/32+SIZE*2 &&
                    e>=diffAnsY/32 && e<=diffAnsY/32+SIZE-1) {
                    start[i][e] = 2;
                }
                else {
                    start[i][e] = Random(1);
                }
            }
        }
        for (int i=diffAnsX/32+2; i<=diffAnsX/32+SIZE*2-2; i++) {
            start[i][diffAnsY/32+1] = 2;
            start[i][diffAnsY/32+3] = 2;
        }
    }
    
    //
    // スタート画面を表示
    //
    void DisplayStart()
    {
        ShowStart();
        Font font{ 24 };
        font(U"Mimic Lights out").drawAt(diffAnsX+3*32-8, diffAnsY+2*32-8, Color{234, 234, 234});
        font(U"Click here to start").drawAt(diffAnsX+3*32-8, diffAnsY+4*32-8, Color{234, 234, 234});
    }
    
    //
    // プレイ中の画面を表示
    //
    void DisplayGame()
    {
        ShowMap();
        ShowAns();
        CheckClear();
        
        Rect{diffAnsX, diffAnsY+SIZE*32, 32}.draw(Color{99, 214, 213});
        Rect{diffAnsX+32, diffAnsY+SIZE*32, 32}.draw(Color{99, 214, 213});
        Rect{diffAnsX+(SIZE-1)*32, diffAnsY+SIZE*32, 32}.draw(Color{99, 214, 213});
        Rect{diffAnsX+(SIZE-0)*32, diffAnsY+SIZE*32, 32}.draw(Color{99, 214, 213});
        Rect{diffAnsX+(SIZE+1)*32, diffAnsY+SIZE*32, 32}.draw(Color{99, 214, 213});
        Rect{diffMapX+3*32, diffAnsY+SIZE*32, 32}.draw(Color{99, 214, 213});
        Rect{diffMapX+4*32, diffAnsY+SIZE*32, 32}.draw(Color{99, 214, 213});

        Font font24{ 24 };
        font24(U"Target").drawAt(diffAnsX+2*32-8, diffAnsY-8, Color{234, 234, 234});
        font24(U"Player").drawAt(diffMapX+1*32-2, diffAnsY-8, Color{234, 234, 234});
        Font font18{ 18 };
        font18(U"Return").drawAt(diffAnsX+4, diffAnsY+(SIZE+1)*32-8, Color{37, 42, 52});
        font18(U"New game").drawAt(diffAnsX+4*32+4, diffAnsY+(SIZE+1)*32-8, Color{37, 42, 52});
        font18(U"Retry").drawAt(diffAnsX+9*32+10, diffAnsY+(SIZE+1)*32-8, Color{37, 42, 52});
    }
    
    //
    // クリア画面を表示
    //
    void DisplayClear()
    {
        ShowMap();
        ShowAns();
        CheckClear();

        for (int i=diffAnsX/32; i<=diffAnsX/32+SIZE*2; i++) {
            Rect{i*32, diffAnsY+SIZE*32, 32}.draw(Color{99, 214, 213});
        }

        Font font24{ 24 };
        font24(U"Target").drawAt(diffAnsX+2*32-8, diffAnsY-8, Color{37, 42, 52});
        font24(U"Player").drawAt(diffMapX+1*32-2, diffAnsY-8, Color{37, 42, 52});
        Font font18{ 18 };
        font18(U"Game clear!!   :   Click here to Start menu").drawAt(diffAnsX+8, diffAnsY+(SIZE+1)*32-8, Color{234, 234, 234});
    }
    
private:
    //
    // マップをコピーする
    //
    void CopyMatrix(Grid<int> from, Grid<int> to)
    {
        int i, e;
        to = from;

        for (i=0; i<SIZE; i++) {
            for (e=0; e<SIZE; e++) {
                to[i][e] = from[i][e];
            }
        }
    }
    
    //
    // 反転処理
    //
    void Flip(int x, int y)
    {
        Console << U"flip[" << x << U"][" << y << U"]";
        map[x][y] = not map[x][y];
        for (int i=x; i<SIZE; i++)    map[i][y] = not map[i][y];
        for (int i=x; i >= 0; i--)    map[i][y] = not map[i][y];
        for (int i=y; i<SIZE; i++)    map[x][i] = not map[x][i];
        for (int i=y; i >= 0; i--)    map[x][i] = not map[x][i];
    }
    
    //
    // スタート画面の背景を表示
    //
    void ShowStart()
    {
        for (int i=0; i<WINDOW_WIDTH; i++) {
            for (int e=0; e<WINDOW_HEIGHT; e++) {
                switch (start[i][e]) {
                    case 0:
                        Rect{i*32, e*32, 32}.draw(Color{234, 234, 234});
                        break;
                    case 1:
                        Rect{i*32, e*32, 32}.draw(Color{234, 70, 102});
                        break;
                    case 2:
                        Rect{i*32, e*32, 32}.draw(Color{37, 42, 52});
                        break;
                    case 3:
                        Rect{i*32, e*32, 32}.draw(Color{99, 214, 213});
                        break;
                }
            }
        }
    }
    
    //
    // マップを表示
    //
    void ShowMap()
    {
        for (int i=0; i<SIZE; i++) {
            for (int e=0; e<SIZE; e++) {
                switch (map[i][e]) {
                    case 0:
                        Rect{i*32+diffMapX, e*32+diffMapY, 32}.draw(Color{234, 234, 234});
                        break;
                    case 1:
                        Rect{i*32+diffMapX, e*32+diffMapY, 32}.draw(Color{234, 70, 102});
                        break;
                }
            }
        }
    }

    //
    // 答えを表示
    //
    void ShowAns()
    {
        for (int i=0; i<SIZE; i++) {
            for (int e=0; e<SIZE; e++) {
                switch (ans[ans_flag][i][e]) {
                    case 0:
                        Rect{i*32+diffAnsX, e*32+diffAnsY, 32}.draw(Color{234, 234, 234});
                        break;
                    case 1:
                        Rect{i*32+diffAnsX, e*32+diffAnsY, 32}.draw(Color{234, 70, 102});
                        break;
                }
            }
        }
    }
    
    //
    // ゲーム終了を判定
    //
    bool CheckClear()
    {
        bool hasClear = true;
        for (int i=0; i<SIZE&&hasClear; i++) {
            for (int e=0; e<SIZE; e++) {
                if (ans[ans_flag][i][e] != map[i][e]) {
                    hasClear = false;
                    break;
                }
            }
        }
        Scene::SetBackground(ColorF{ 0.031, 0.851, 0.839, 1.0 });
        return hasClear;
    }
};





void Main()
{
    Window::Resize(WINDOW_WIDTH*32, WINDOW_HEIGHT*32);
    Window::SetTitle(U"Mimic Lights Out");
    Scene::SetBackground(ColorF{ 0.145, 0.165, 0.204, 1.0 });
    
    GameBoard gameBoard;
    gameBoard.Init();
    
    int game_state = 2;
	while (System::Update())
	{
        if (game_state == 0) {
            gameBoard.DisplayStart();
        }
        else if (game_state == 1) {
            gameBoard.DisplayGame();
        }
        else if (game_state == 2) {
            gameBoard.DisplayClear();
        }
	}
}



//
// = アドバイス =
// アプリケーションをビルドして実行するたびにファイルアクセス許可のダイアログが表示されるのを避けたい場合、
// プロジェクトのフォルダを ユーザ/(ユーザ名)/アプリケーション/ などに移動させると良いです。
// Web カメラ、マイク使用時の許可ダイアログは消せません。
//
// チュートリアル
// https://siv3d.github.io/ja-jp/tutorial/tutorial/
//
// Tutorial
// https://siv3d.github.io/tutorial/tutorial/
//
// Siv3D コミュニティへの参加（Discord などで気軽に質問や交流, 最新情報の入手ができます）
// https://siv3d.github.io/ja-jp/community/community/
//
// Siv3D User Community
// https://siv3d.github.io/community/community/
//
// 新機能の提案やバグの報告 | Feedback
// https://siv3d.github.io/ja-jp/develop/report/
//
// Sponsoring Siv3D
// https://github.com/sponsors/Reputeless
//
