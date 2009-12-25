main 
	if talk
	then
		if quest * 3 * 0
		then
			if exist_item * 5 * 1 " Brain.item.hs"
			then
				speak * 27
				setQuestState * 3 * 2  * 28
				giftItem * 0
			else
				speak * 26
			end
		end
		
		if quest * 3 * -1
		then
			speak * 26
			newQuest * 3 
			setQuestName * 3  * 29
			setQuestState * 3 * 0  * 26
			setQuestPosition * 3 * 47 * 71 " BurialGround.map.hs"
		end
	end
end 
