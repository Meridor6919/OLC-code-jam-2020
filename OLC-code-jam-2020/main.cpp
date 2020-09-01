#include <Windows.h>
#include <thread>

LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE prev_hinstance, LPSTR cmd_line, int show_cmd)
{
	WNDCLASSEX wc{ sizeof(WNDCLASSEX), CS_VREDRAW | CS_HREDRAW,  WndProc, 0 , 0, hinstance, 0, 0, 0, 0, L"OLC", 0 };
	if (!RegisterClassEx(&wc))
	{
		return false;
	}
	RECT dr;
	GetWindowRect(GetDesktopWindow(), &dr);
	HWND hwnd = CreateWindowEx(NULL, L"OLC", L"The great machine", WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX, (dr.right - 800) / 2, (dr.bottom - 600) / 2, 800, 600, 0, 0, hinstance, 0);

	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);

	MSG msg = { 0 };
	HANDLE timer = CreateWaitableTimer(0, true, L"MainLoopTimer");
	LARGE_INTEGER large_int;
	large_int.QuadPart = -166666; //60 Hz
	
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			SetWaitableTimer(timer, &large_int, 0, 0, 0, 0);
			//Update(delta_time);
			//Render(delta_time);
			WaitForSingleObject(timer, INFINITE);
		}
	}
	return static_cast<int>(msg.wParam);
}

LRESULT __stdcall WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
	{
		//DirectX::Mouse::ProcessMessage(msg, wparam, lparam);
		break;
	}
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		//DirectX::Keyboard::ProcessMessage(msg, wparam, lparam);
		break;
	}
	default:
	{
		break;
	}
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}
