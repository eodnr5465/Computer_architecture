# Computer_architecture
computer의 기본 구성을 c++로 구성하고 구성한 architecture를 바탕으로 성능개선을 수행함.

소개
이번 마이크로프로세서 시간을 통해 c++의 객체지향 언어를 사용하여 비메모리 반도체 영역을 직접 설계하였다. 그 결과 program memory에 있는 binary code들이 어떻게 decoding 되는지에 대한 과정과 ALU(Arithmetic Logic Unit)에 operand들이 들어와 data를 처리하여 data memory인 flash memory에 저장되는 과정에 대한 이해도를 높일 수 있었다.
 그 동안 문제되어 왔던 power consumption 즉, 발열 문제나 연산 처리속도와 같은 문제를 하드웨어로 그 성능을 발전시킬 수 있다. 하지만 그 한계는 반드시 존재할 수밖에 없으며 이러한 문제를 소프트웨어로서 해결을 해야 한다. 따라서 CPU내부에서 data들이 처리되는 과정에 대한 이해는 program을 개발하거나 칩 구조를 설계하는데 있어서 필수적인 역량이 될 것이다.
따라서 수업시간에 설계해 왔던 TPU를 이용하여 직접 3x3 Matrix 곱셈 연산을 하는 binary code를 구현해 보면서 어떻게 하면 소프트웨어로 비교적 높은 성능을 구현할 수 있을지 고민해 보았다.
그 방법의 첫번째로는 하나의 binary code를 해석하고 그에 맞는 연산을 하는 과정에서 clock의 소모를 줄일 수 있게 기본 연산 기능에 대한 성능을 높이는 것이 있다. 만약 chip을 설계 즉, 논리회로 합성하는 과정에서 transistor 간의 연결로 3x3 matrix 연산을 구현할 수 있다면 가장 좋겠지만 너무 특수 목적용이고 과제의 취지에 맞지 않는다.
두 번째 방법으로 기존의 연산자 (mov0, mov1, mov3, add, sub, mul)에 대한 binary code 구조를 이해하고 transistor의 switching을 최소화하는program code를 구현하여 power consumption을 최소화하는 code를 구현하는 방향으로 설정하였다.

방법 1. 같은 명령어를 최대한 몰아서 실행시키기
16bit의 제약에 같은 명령어에 대해서는 최대한 한꺼번에 처리하기 위해 1번째 matrix는 register에 고정시키고 2번째 피 연산 matrix를 2개의 column씩 register에 올려서 연산함.

방법 2. Flash memory에 접근하는 횟수를 줄임
2번째 matrix의 마지막 column에 대한 연산은 1번째 matrix에 연산 결과를 저장하여 register내에서 모든 연산을 최대한 시행 후 flash memory에 loading시킴.
방법 3. Don’t care bit의 switching을 최소화
mul이나 add와 같은 명령어는 LSB 4bit는 don’t care bit 이기 때문에 최대한 이전 실행 명령과 동일하게 4개의 bit를 switching 하지 않음.


![image](https://user-images.githubusercontent.com/58467557/70104215-b23a1f80-1680-11ea-8dd6-672dbe02798d.png)
<그림1> binary code 생성
그림 1 과 같이 머리속에 생각한 logic을 binary code로 작성을 하여 program binary code를 생성시킨다.

![image](https://user-images.githubusercontent.com/58467557/70104220-b49c7980-1680-11ea-800a-86afa59d1125.png)
<그림 2> binary code fetch
그림 2 와 같이 CCode를 통해 binary code를 16bit 한 줄마다 CDecode로 가져온 후 CDecode에서 16bit의 program data를 opcode와 op1, op2를 각각 4bit, 4bit, 8bit로 나누어서 각bit 정보에 대한 구분을 지어 준다.

![image](https://user-images.githubusercontent.com/58467557/70104231-ba925a80-1680-11ea-81a7-55c4bf86d82e.png)
<그림 3> Execute
그림 3과 같이 CDecode에서 해석된 opcode에 따라 수행하는 동작이 달라지는데 opcode가 4bit 이므로 총 8가지의 동작을 할 수 있는데 이중 7가지를 선언하여 사용한다. 이때 CExecute의 행위에 따라 data들이 이동하고 연산을 하기 때문에 register file 과 memory file에 영향을 줄 수 있다.
