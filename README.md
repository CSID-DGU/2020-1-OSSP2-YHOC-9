#### 공개 SW 프로젝트 - 02
# Proposal_YHOC

<div style="text-align: right">
YHOC - YouHaveOurCode</div>  
<div style="text-align: right">
담당교수자 : 김동호 교수님</div> 
<div style="text-align: right">
팀장 : 장창현</div> 
<div style="text-align: right">
팀원 : 김윤호, 한소희</div> 
</div>

<div style="page-break-after: always;"></div>

## 1. Subject | PAC-MAC 게임 개량

본 팀은 공개소프트웨어 프로젝트 수행 경험 축적을 목적으로 복잡하지 않은 게임에서 개선점을 찾아,  
수정 및 추가하는 방향으로 PAC-MAN게임을 선정했다.  

- 주 사용언어 : C
- 주요 Library : SDL Package
- 개발환경 : Oracle VirtualBox - Ubuntu, Visual Studio Code(IDE) 
- License : MIT

#### Code Source

- Original PACMAN version : https://github.com/TroyShaw/Pacman  
- 2018 공개SW 프로젝트 : https://github.com/CSID-DGU/2018-1-OSSP-FCTC-4  
- 2019 공개SW 프로젝트 : https://github.com/CSID-DGU/2019-1-OSSPC-Lemonwater-2

#### Image Source

- Icons made by [Darius Dan](https://www.flaticon.com/authors/darius-dan) from https://www.flaticon.com/  

- Icons made by https://www.flaticon.com/authors/darius-dan Darius Dan

## 2. Getting Started

1. Install SDL

```
sudo apt-get update

sudo apt-get install libsdl-ttf2.0-0 libsdl-sound1.2 libsdl-mixer1.2

sudo apt-get install libsdl-image1.2 libsdl-gfx1.2-5 libsdl1.2-dev
```

2. Compile

```
cd (make파일이 있는 폴더 경로)

make
```

3. Execute

```
./game_Debug
```

## 3. Problem & Solution

- 전반적인 게임설명 미흡  
> 기본 메뉴화면에서 게임설명 항목 추가

- 고스트가 특정 상황에서, 플레이어를 따라오지 않는 이상 로직  
> 고스트의 이상패턴 파악 및 로직 수정

- ESC키 입력시, 경고나 알림없이 즉시 종료되는 문제  
- 일시정지 기능 부재  
- 멀티모드의 서버를 개설하고 대기하는 상태 중 취소 기능의 부재 
> ESC키 입력시, 게임 일시정지 후 게임종료 확인창 출력 (QUIT / MENU / BACK) 및 관련 기능 구현

## 4. Additional 

- 멀티모드에서 타 플레이어가 Ghost 역할을 수행하는 **추격모드** 추가 및 구현

- Window창 크기 및 해상도 조절 가능케 함

- 새로운 이미지 / 글꼴 사용으로 게임에 신선함 부여

## 5. GAME

게임시작메뉴 / 게임설명

<img width=300 src="https://user-images.githubusercontent.com/54177123/85509729-70c78c80-b631-11ea-9706-abe313ee2978.png"></img>
<img width=300 src="https://user-images.githubusercontent.com/54177123/85509752-79b85e00-b631-11ea-8127-79b197a9b1ae.png"></img>

일시정지 / 멀티모드

<img width=300 src="https://user-images.githubusercontent.com/54177123/85509792-8341c600-b631-11ea-8e28-65b80dae1488.png"></img>
<img width=300 src="https://user-images.githubusercontent.com/54177123/85509782-8046d580-b631-11ea-90f5-2c522a3484b3.png"></img>

게임화면

<img width=300 src="https://i.imgur.com/s6wYnNl.png"></img>
<img width=300 src="https://i.imgur.com/jDg6bUq.png"></img>

## 6. License

MIT License

Copyright (c) 2020 CSID DGU - YHOC

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

SDL1.2 Library : [GNU Lesser General Public License (LGPLv2)](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.en.html)
