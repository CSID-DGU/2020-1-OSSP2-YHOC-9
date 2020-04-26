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

본 프로젝트는 다음의 2018년 공개SW 프로젝트 개량한다.  

[2018 공개SW 프로젝트 - FCTC](https://github.com/CSID-DGU/2018-1-OSSP-FCTC-4)  

해당 프로젝트는 1980년 회사 남코에서 기획 및 제작한 PAC-MAN 게임을 개량하였으며,  
멀티모드와 아이템 요소를 추가한 프로젝트이다.  

- 주 사용언어 : C
- 주요 Library : SDL Package
- 개발환경 : Oracle VirtualBox - Ubuntu, Visual Studio Code(IDE) 
- License : MIT

## 2. Problems

1. 고스트가 특정 상황에서, 플레이어를 따라오지 않는 이상 로직  
2. ESC키 입력시, 경고나 알림없이 즉시 종료되는 문제  
3. 일시정지 기능 부재  
4. 전반적인 게임설명 미흡  
5. 멀티모드의 서버를 개설하고 대기하는 상태 중 취소 기능의 부재  

## 3. Solution

1. 고스트의 이상패턴 파악 및 로직 수정  
2. ESC키 입력시, 게임 일시정지 후 게임종료 확인창 출력 (Exit / Continue) 및 관련 기능 구현 
3. 기본 메뉴화면에서 게임설명 항목 추가
4. 멀티모드 대기 중 취소 기능 추가 및 구현
5. 재미요소로서, 멀티모드에서 타 플레이어가 Ghost 역할을 수행하는 **추격모드** 추가 및 구현
6. Window창 크기 및 해상도 조절 문제 해결

<div style="page-break-after: always;"></div>

## 4. Timeline 

| |1주차|2주차|3주차|4주차|5주차|6주차|7주차|8주차|
|---|---|---|---|---|---|---|---|---|
|기획회의|>>>>||||||||
||||||||||
|SDL 및 배경지식 학습||>>>>|||||||
||||||||||
|코드분석|||>>>>|>>>>|||||
||||||||||
|수정 & 구현 (TASK_01)|||||>>>>||||
||||||||||
|개발 & 구현 (TASK_02)||||||>>>>|>>>>||
||||||||||
|테스트 / 발표준비||||||||>>>>|
||||||||||
||||||||||

### **Detail_**

- 기획회의
    - 프로젝트 선정 및 제안서 작성

- SDL 및 배경지식 학습
    - 코드 분석에 필요한 배경지식 학습

- 코드분석
    - 기존 프로젝트 코드 분석 및 흐름 파악

- 수정 & 구현 (TASK_01)
    - 고스트 이상로직 수정
    - ESC키 이벤트 추가 구현
    - 게임설명 컨텐츠 추가
    - Window Resizing 문제 해결

- 개발 & 구현 (TASK_02)
    - 멀티 모드 대기 중 취소기능 추가 구현
    - (멀티모드) 추격모드 구현

- 테스팅 / 발표준비
    - 게임상의 문제 확인 및 최종 마무리
    - 최종 발표 및 프로젝트 종료
   
<div style="page-break-after: always;"></div>

## 5. Roles

공동 수행 : 기획회의 / 배경지식 학습 / 코드분석

- 장창현 : ESC키 이벤트 추가 구현, 서버 개선

- 김윤호 : 고스트 이상로직 수정, 서버 개선

- 한소희 : 클라이언트 개선 (게임 설명, 윈도우 리사이징), 서버 개선

이후, 코드 분석을 통해 멀티모드 개발에 대한 역할 분담을 구체화할 예정

## 6. License

SDL1.2 Library : [GNU Lesser General Public License (LGPLv2)](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.en.html)

LGPL 코드는 미수정시 사용 명시만을 요구. (본 프로젝트에서는 수정하지 않고 SDL 라이브러리 이용할 예정)

기존 프로젝트 라이센스 : MIT

> 본 프로젝트는 MIT License를 따르며, SDL1.2 library 사용을 명시토록 한다.