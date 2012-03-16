main 
	if hour * 21 * 23 
	then
		if getState * 0
		then
			if rand * 50
			then
				setState * 1 * 0
			else
				setState * 1 * 4
			end
		end
	else
		if hour * 0 * 6 
		then
			if getState * 0
			then
				if rand * 50
				then
					setState * 1 * 0
				else
					setState * 1 * 4
				end
			end
		else
			if getState * 1
			then
				setState * 0 * 0
			end
		end
	end
end 