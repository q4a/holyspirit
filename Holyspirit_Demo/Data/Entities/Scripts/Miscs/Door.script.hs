
main 
	incrementVariable * 4 * time
	if variable * 1 * 0
		variable_bigger * 4 * 1
	then
		setActif * 1

		if distance * 1
		then else

			if talk
			then
				if variable * 0 * 1
				then
					variable * 0 * 0
					setState * 1  * 0
					variable * 4 * 0
				else
					variable * 0 * 1
					setState * 3  * 0
					variable * 4 * 0
				end
			
				stop_talk
			end
		end
	else 
		if variable * 1 * 1
		then
			setActif * 0
		else
			setActif * 1
			if talk
			then
				tell * 175 * 5000
			end
		end
	end
	
	if variable * 0 * 1
	then
		setCollision * 0

		if getState * 3
		then else
			setState * 3 * 0
		end
	else
		setCollision * 1

		if getState * 1
		then else
			if getState * 0
			then else
				setState * 1 * 0
			end
		end
	end
end 
