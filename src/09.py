def extrapolate_next_num(nums: list[int], direction: str):
    if len(nums) == 0:
        return 0

    nums2 = [0] * (len(nums) - 1)
    for i in range(1, len(nums)):
        nums2[i-1] = nums[i] - nums[i - 1]
    if direction == "right":
        return nums[-1] + extrapolate_next_num(nums2, direction)
    return nums[0] - extrapolate_next_num(nums2, direction)


prev_sum = 0
next_sum = 0

with open("./input/9") as f:
    while line := f.readline():
        nums = [int(x) for x in line.split()]
        prev_sum += extrapolate_next_num(nums, "left")
        next_sum += extrapolate_next_num(nums, "right")

print(prev_sum)
print(next_sum)
