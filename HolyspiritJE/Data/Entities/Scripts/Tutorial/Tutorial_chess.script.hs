main 
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
		tell * 35 * 3000
	end
end 