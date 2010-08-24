
main 
	if variable * 1 * 0
	then
		setActif * 1

		if talk
		then
			if variable * 0 * 1
			then
				variable * 0 * 0
				setState * 0  * 0
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
	else
		setState * 0 
		setCollision * 1
	end
end 
