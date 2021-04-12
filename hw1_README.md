## Description

C언어 컴파일러 제작을 위한 기초 단계로 진행하였습니다.  
간단한 수식을 위한 신텍스 분석기(Syntax Analysis)를 제작할 예정이며, 
사용하는 토큰은 "+", "*", "(", ")", "0~9", "END"입니다
<br><br>

신택스 그래프는 다음과 같다<br><br>
![syntaxgraph](https://user-images.githubusercontent.com/29463639/114151213-dcc47880-9957-11eb-9928-7b5fdfb76090.jpg)  
\<expression>은 \<term>이 한 개이거나 혹은 여러 개의 \<term> 들의 더하기로 이루어져 있다  
\<term>은 \<factor> 가 한 개이거나 혹은 여러 개의 \<factor> 들의 곱하기로 이루어져 있다  
\<factor>는 \<number>이거나 혹은 여는 소괄오 뒤에 \<expression>과 닫는 소괄호로 이루어져 있다  
  
  
## Algolithm  
![화면 캡처 2021-04-12 145345](https://user-images.githubusercontent.com/29463639/114346739-e68bed00-9b9e-11eb-9f7f-832039f1c9f5.jpg)  
get_token() 함수의 알고리즘 플로우입니다  


### 목표
* [x] 한 자리 정수끼리의 계산(0~9)(21.04.10. 완료)  
* [x] 두 자리 이상의 정수끼리 계산(21.04.12. 완료)  
* [x] 실수 계산(21.04.12. 완료)  
