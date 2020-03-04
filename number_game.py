# 랜덤함수를 불러옵니다.
from random import randint

# 변수 초기값
strike_count = 0    # 스트라이크 카운팅 초기 값
ball_count = 0      # 볼 카운팅 초기 값
tries = 0            # 시도 횟수


# 임의의 숫자 뽑기
numbers = []
while len(numbers) < 3:
    new_numbers = randint(0, 9)
    if new_numbers not in numbers:
        numbers.append(new_numbers)

print("0과 9 사이의 서로 다른 세 숫자를 랜덤한 순서로 뽑았습니다.")
print("세 수를 하나씩 차례대로 입력하세요.")

# 사용자로 부터 숫자를 입력 받습니다.
while strike_count < 3:
    thinking = []   # 입력된 숫자가 위치할 곳
    i = 0  # 인덱싱 변수
    while len(thinking) < 3:
        user_input = int(input("%d번째 수를 입력하세요: " % (i + 1)))
        i += 1
        if user_input < 0 or user_input > 9:
            print("범위를 벗어나는 수입니다. 다시 입력해주세요")
            # 0과 9사이의 숫자를 입력받으면 다시 입력을 받습니다.
        elif user_input in thinking:
            print("중복되는 수 입니다. 다시 입력해주세요.")
            # 입력받은 숫자가 이미 입력 받았을 경우 다시 입력을 받습니다.
        else:
            thinking.append(user_input)
            # 문제가 없으면 입력합니다.

    # 횟수 세기
    i = 0  # 인덱싱 변수
    strike_count = 0    # 스트라이크 카운트 초기화
    ball_count = 0      # 볼 카운트 초기화

    while i < 3:
        if thinking[i] == numbers[i]:
            strike_count += 1
        elif thinking[i] in numbers:
            ball_count += 1
        i += 1

    print("%dS %dB" % (strike_count, ball_count))
    tries += 1  # 시도 횟수를 1회 늘립니다.

# 결과창
print("축하합니다. %d번만에 세 숫자의 값과 위치를 모두 맞추셨습니다." % (tries))
