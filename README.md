# Digital Signal Processing
## 1. Quantization
**Quantization**이란 여러 개의 samples을 하나의 대표값으로 mapping하는 과정이다. 그래서 원본 sample 값에 비해 손실이 생길 수밖에 없다. 
![image](https://github.com/user-attachments/assets/cd32d449-ab9b-422e-9de7-74a4dbd4d953)

Quantization 대상에 따라 다음과 같이 나눌 수 있다.
- Scalar: 여러 개의 Scalar Samples을 하나의 Representation level로 mapping
- Vector: 여러 개의 Vector Samples을 하나의 Representation level로 mapping

또한, Quantization level 간격에 따라 다음과 같이 나눌 수 있다.
- Uniform: Reconstruction levels이 균일하게 분포
- Non-uniform: Reconstruction levels이 비균일하게 분포

### 1.1 Scalar Uniform Quantization
![image](https://github.com/user-attachments/assets/c8ae4c3f-885c-4ee0-87d9-f6d4093559af)

### 1.2 Scalar Non-uniform Quantization
여기서는 Lloyd-Max quantizer을 사용한다. 이는 sample의 분포를 고려하여 Quantization Error가 최소가 되도록 각 decision level과 representation level을 결정하는 것이다. 확률 밀도가 높은 구간에서는 step size을 작게, 확률 밀도가 낮은 구간에서는 step size을 크게 한다.

![image](https://github.com/user-attachments/assets/a0a343ac-b94e-4c1f-b45a-4d77478a750d)
![image](https://github.com/user-attachments/assets/d39b8df4-0716-4b82-b7a3-1ec6c5dcede2)
Quantization Error가 threshold보다 작아질 때까지, 또는 정해진 횟수만큼 모두 수행할 때까지 $$d_i$$와 $$r_i$$을 반복적으로 업데이트한다. 이때 Reconstruction level과 Quantization level의 pair(Codebook)을 저장하여, Codebookd을 참조하여 역양자화를 수행한다.

### 1.3 Vector Non-uniform Quantization
여기서는 K-means Algorithm을 이용하여 그룹 범위와 각 그룹의 reconstructed vector을 설정한다. 처음에는 K개의 reconstructed vector($$V_r$$)의 초기값을 설정한다.

![image](https://github.com/user-attachments/assets/951eb606-b7d8-43fa-87be-d7a1d70b384f)

이어서 각 Input vector와 $$V_r$$ 간 길이를 측정하고, 최소 길이 상에 위치한 $$V_r$$ 주변의 input vectors을 하나의 그룹으로 묶는다. 

![image](https://github.com/user-attachments/assets/194ea234-cb6f-4754-b949-eda3782f6816)

그리곤서 
