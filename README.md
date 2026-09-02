# ImageProcessor 과제 제출

## 구현 항목

- GrayScale 변환 (평균값, 가중치 모두 구현)
- 밝기 / 대비 조절
- 이진화
- 3x3 convolution (blur, sharpen 모두 구현)
- 히스토그램 분석
- 이미지 크롭 / 리사이즈
- 상하/좌우 반전

- thread사용, LOG 구현(싱글톤 사용), 추상클래스 사용(FilterBase), Pipeline 구현

## 실행 명령어 (예시)

```powershell
# Grayscale 변환(평균)
.\x64\Release\ImageProcessor.exe -i .\Resource\image.bmp -o .\Resource\output_grayscale.bmp -l  .\log.txt -f grayscale

# Grayscale 변환(가중치)
.\x64\Release\ImageProcessor.exe -i .\Resource\image.bmp -o .\Resource\output_grayscale_weight.bmp -l  .\log.txt -f grayscale_weight
```

```powershell
# 밝기 / 대비 조절
.\x64\Release\ImageProcessor.exe -i .\Resource\image.bmp -o .\Resource\output_bright_contrast.bmp -l  .\log.txt -b 200 -c 1.3
```

```powershell
# 이진화(칼라)
.\x64\Release\ImageProcessor.exe -i .\Resource\image.bmp -o .\Resource\output_threshold.bmp -l  .\log.txt -t 128

# 이진화(흑백)
.\x64\Release\ImageProcessor.exe -i .\Resource\image.bmp -o .\Resource\output_threshold_noColor.bmp -l  .\log.txt -f grayscale -t 128
```

```powershell
# 3x3 (blur)
.\x64\Release\ImageProcessor.exe -i .\Resource\5_checkerboard.bmp -o .\Resource\output_33blur.bmp -l  .\log.txt -f blur -s 0

# 3x3 (sharpen
.\x64\Release\ImageProcessor.exe -i .\Resource\5_checkerboard.bmp -o .\Resource\output_sharpen.bmp -l  .\log.txt -f blur -s 1
```

```powershell
# 히스토그램
.\x64\Release\ImageProcessor.exe -i .\Resource\image.bmp -o .\Resource\output.bmp -l .\log.txt -hi 1
```

```powershell
# 이미지 리사이즈
.\x64\Release\ImageProcessor.exe -i .\Resource\image.bmp -o .\Resource\output_resize.bmp -l  .\log.txt -re 2000,1000

# 이미지 크롭
.\x64\Release\ImageProcessor.exe -i .\Resource\image.bmp -o .\Resource\output_crop.bmp -l  .\log.txt -cr 100,100,500,500
```

```powershell
# 상하반전
.\x64\Release\ImageProcessor.exe -i .\Resource\image.bmp -o .\Resource\output_verFlip.bmp -l  .\log.txt -fl 1,0

# 좌우반전
.\x64\Release\ImageProcessor.exe -i .\Resource\image.bmp -o .\Resource\output_horFlip.bmp -l  .\log.txt -fl 0,1

# 상하/좌우 반전
.\x64\Release\ImageProcessor.exe -i .\Resource\image.bmp -o .\Resource\output_Flip.bmp -l  .\log.txt -fl 1,1
```

```powershell
# 필터 파이프라인 체인(고급)
.\x64\Release\ImageProcessor.exe -i .\Resource\image.bmp -o .\Resource\output_pipeline.bmp -l .\log.txt -p grayscale/brightness:100/crop:100,100,700,700/resize:1000,2000
```
