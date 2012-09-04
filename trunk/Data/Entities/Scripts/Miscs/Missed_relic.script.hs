
main 
	if interact
	then
		tell * 178 * 5000
		setState * 3
		newQuest * 10
		setQuestName * 10 * 60
		setQuestState * 10 * 0 * 61

		if exist_item * 3 * 0 " Quest_relic4.item.hs"
		then
			setQuestState * 10 * 1 * 62
			setQuestActif * 10 * 0
		end
	end
end 
