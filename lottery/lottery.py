from random import randint


# 무작위로 정렬된 1 - 45 사이의 숫자 여섯개 뽑기
def generate_numbers():
    # 랜덤한 숫자를 뽑고 numbers에 저장합니다.
    numbers = []
    while len(numbers) < 6:
        new_number = randint(1, 45)
        if new_number not in numbers:
            numbers.append(new_number)
    # numbers를 정렬함으로써 후에 비교하기 편하게끔 함
    numbers.sort()
    return numbers


# 보너스까지 포함해 7개 숫자 뽑기
# 정렬된 6개의 당첨 번호와 1개의 보너스 번호 리스트를 리턴

def draw_winning_numbers():
    # 당첨숫자가 들어갈 리스트는 winning_numebrs입니다.
    # winning_numbers도 마찬가지로 랜덤한 숫자를 뽑기 때문에 위의 함수를 빌려옵니다.
    winning_numbers = generate_numbers()
    # 보너스 숫자는 뽑고 맨 뒤에 붙입니다.
    while len(winning_numbers) < 7:
        bonus_number = randint(1, 45)
        if bonus_number not in winning_numbers:
            winning_numbers.append(bonus_number)

    return winning_numbers


# 두 리스트에서 중복되는 숫자가 몇개인지 구하기
def count_matching_numbers(list1, list2):
    # 몇 개 맞췄는지에 대한 카운팅 변수
    count = 0
    # 첫번째 리스트 부터 비교합니다.
    for num in list1:
        if num in list2:
            count += 1

    return count


# 로또 등수 확인하기
def check(numbers, winning_numbers):
    correct = count_matching_numbers(numbers, winning_numbers)

    if correct == 6:
        return 1000000000
    elif correct == 5 and winning_numbers[6] in numbers:
        return 50000000
    elif correct == 5:
        return 1000000
    elif correct == 4:
        return 50000
    elif correct == 3:
        return 5000
    else:
        return 0
