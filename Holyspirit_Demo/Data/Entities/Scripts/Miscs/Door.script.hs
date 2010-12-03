
main 
	if variable * 1 * 0
	then
		setActif * 1

		if talk
		then
			if variable * 0 * 1
			then
				variable * 0 * 0
				setState * 1  * 0
			else
				variable * 0 * 1
				setState * 3  * 0
			end
			
			stop_talk
		end
	else
		setActif * 0
	end
	
	if variable * 0 * 1
	then
		setState * 3
		setCollision * 0

		if variable * 2 * 0
		then
			setState * 3 * 0
		end

		variable * 2 * 1
	else
		setCollision * 1

		if variable * 2 * 1
		then
			setState * 1 * 0
		end

		variable * 2 * 0
	end
end 
