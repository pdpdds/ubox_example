import string
import sys

def is_hex(s):
	try:
		int(s, 16)
		return True
	except ValueError:
		return False

f1 = open('12_debugger.sym','w') # 저장할 파일 이름
with open('12_debugger.map','r') as f2: # 원본 심볼 파일
	for line in f2: # 원본 파일에서 한줄씩 읽는다
		line1 = line.strip()
		words = line1.split() # 라인을 단어로 구분해서 words 리스트 변수에 저장한다
		if len(words) > 1:
			if is_hex(words[0]): # 숫자로 시작하는 경우에만 기록한다
				f1.write(words[1] + ': equ ' + words[0] + "H\n")
f2.close() # 생성 파일의 핸들을 닫는다
f1.close() # 원본 파일의 핸들을 닫는다
exit()
