import re

CARD_MAX = 199

cards_won = [0] * CARD_MAX
total = 0
rx_card = re.compile(r"Card\s+(\d+)")
rx_nums = re.compile(r"\d+|\|")

with open("input/4") as f:
    for line in f:
        card_num_match = rx_card.match(line)
        card_num = card_num_match and int(card_num_match.group(1)) or 0
        nums = rx_nums.findall(line)
        pipe = nums.index("|")
        wins, actual = nums[1:pipe], nums[pipe + 1:]
        matches = sum([num in wins for num in actual])
        cards_won[card_num] += 1
        for i in range(card_num + 1, card_num + matches + 1):
            cards_won[i] += cards_won[card_num]

total = sum(cards_won)
print(total)
