main 
	if talk
	then
		if quest * 2 * 2
		   variable * 0 * 0
		then
			if quest * 1 * 0
			then
				speak * 11
			end
			if quest * 1 * 1
			then
				speak * 12
			end
			if quest * 1 * 2
			then
				speak * 13
			end

			if quest * 1 * 3
			then
				speak * 15
				setQuestState * 1 * 4  * 17
				giftItem * 0
			end
			
			if quest * 1 * -1
			then
				if variable * 0 * 0
				then
					playSound * 1
					variable * 0 * 1
				end

				speak * 16
				newQuest * 1 
				setQuestName * 1  * 18
				setQuestState * 1 * 0  * 11
				setQuestPosition * 1 * 20 * 83 " TerribleDungeon.map.hs"
			end
		else
			variable * 0 * 1
			if quest * 2 * 1
			then
				speak * 24
				setQuestState * 2 * 2  * 23
				addCash * 2000
			end
			
			if quest * 2 * 0
			then
				speak * 22
			end
			
			if quest * 2 * -1
			then
				speak * 21
				newQuest * 2 
				setQuestName * 2  * 20
				setQuestState * 2 * 0  * 21
				setQuestPosition * 2 * 24 * 61 " ArableLand3.map.hs"
			end
		end
	else
		variable * 0 * 0
	end
end 
