# C언어로 기본 K-means 알고리즘 구현

#### 1. 구현 순서
- CSV 파일을 데이터로 받아오기
- 입력 받은 n_cluster의 수 따라 무작위 데이터들 중 n개를 Centroid로 설정
- 클러스터링을 하여 유크리드 거리 계산으로 Centroid의 위치를 이동
- Centroid의 위치가 이동하기 전과 동일할 때까지 클러스터링
- kmeans 평가 중의 하나인 실루엣 분석을 구현
- 결과를 출력
#### 2. 실행 방법
  $ .\KmeansAlgoritmC.exe iris.csv

#### 3. 실행 결과
![실행 결과](실행_결과.png)