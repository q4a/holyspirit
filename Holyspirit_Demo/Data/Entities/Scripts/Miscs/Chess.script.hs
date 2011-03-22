main 

	if variable * 1 * 0
	then
		if alive
		then
			if talk
			variable * 0 * 0
			then
				gift_all_items
				variable * 0 * 1
			end
		end

		if variable * 0 * 1
		then
			setActif * 0
			setState * 3
		end

		if variable * 0 * 0
			getState * 3
		then
			setActif * 1
			setState * 0 * 0
		end
	else
		if talk
		then
			tell * 175 * 5000
			stop_talk
		end
	end
end 