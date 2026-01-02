#include "SoundManager.h"
#include "MyMacro.h"

SoundManager::SoundManager()
	: m_pSound	()
{
	//インスタンス生成.
	for( int i = 0; i < enList::max; i++ )
	{
		m_pSound[i] = new Sound();
	}
}

SoundManager::~SoundManager()
{
	Release();

	//インスタンス破棄.
	for( int i = enList::max - 1; i >= 0; i-- )
	{
		SAFE_DELETE( m_pSound[i] );
	}
}

//サウンドデータ読込関数.
bool SoundManager::Load( HWND hWnd )
{
	struct SoundList
	{
		int listNo;				//enList列挙型を設定.
		const TCHAR path[256];	//ファイルの名前(パス付き).
		const TCHAR alias[32];	//エイリアス名.
	};
	SoundList SList[] =
	{
		{ enList::SE_Enter,		_T( "Data\\Sound\\SE\\Enter.mp3" ),				_T("SE_Enter") },
		{ enList::SE_Exp,		_T( "Data\\Sound\\SE\\explosion.mp3" ),			_T( "SE_Exp" ) },
		{ enList::SE_Shot,		_T( "Data\\Sound\\SE\\Shot.mp3" ),				_T( "SE_Shot" )},
		{ enList::BGM_Ending,	_T( "Data\\Sound\\BGM\\ending.mp3" ),			_T( "BGM_End"	)},
		{ enList::BGM_Over,		_T( "Data\\Sound\\BGM\\gameover.mp3" ),			_T( "BGM_Over"	)},
		{ enList::BGM_Main,		_T( "Data\\Sound\\BGM\\gamemain.mp3" ),			_T( "BGM_Main"	)},
		{ enList::BGM_Title,	_T( "Data\\Sound\\BGM\\title.mp3" ),			_T( "BGM_Title"	)},
	};
	//配列の最大要素数を算出 (配列全体のサイズ/配列1つ分のサイズ).
	int list_max = sizeof( SList ) / sizeof( SList[0] );
	for( int i = 0; i < list_max; i++ )
	{
		if( m_pSound[SList[i].listNo]->Open(
			SList[i].path,
			SList[i].alias,
			hWnd ) == false )
		{
			return false;
		}
	}

	return true;
}

//サウンドデータ解放関数.
void SoundManager::Release()
{
	//開いた時と逆順で閉じる.
	for( int i = enList::max - 1; i >= 0; i-- )
	{
		if( m_pSound[i] != nullptr )
		{
			m_pSound[i]->Close();
		}
	}
}
