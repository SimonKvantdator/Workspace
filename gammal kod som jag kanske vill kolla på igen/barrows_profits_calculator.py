chaos_rune = 69 # [gp]
blood_rune = 388
death_rune = 204
mind_rune = 4

barrows_teleport = 2470
prayer_potion = 9188
shark = 795

income = 174 \
	+ 237 * chaos_rune \
	+ 78 * death_rune \
	+ 312 * mind_rune \
	+ 42 * blood_rune

expenses = 2 * shark \
	+ 3 * prayer_potion \
	+ 1 * barrows_teleport
	
print(income - expenses)
