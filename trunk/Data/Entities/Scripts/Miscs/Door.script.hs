
main 
	incrementVariable * 4 * time
	if variable * 1 * 0
		variable_bigger * 4 * 1
	then
		setActif * 1

		if distance * 1
		then else

			if interact
			then
				if variable * 0 * 1
				then
					variable * 0 * 0
					setState * 1  * 0
					variable * 4 * 0
				else
					variable * 0 * 1
					setState * 2  * 0
					variable * 4 * 0
				end
			
				stop_interact
			end
		end
	else 
		if variable * 1 * 1
		then
			setActif * 0
		else
			
			if variable_bigger * 4 * 1
			then
				setActif * 1
			else
				setActif * 0
			end

			if interact
				variable * 1 * 2
			then
				tell * 174 * 5000

				stop_interact
			end
		end
	end
	
	if variable * 0 * 1
	then
		setCollision * 0

		if getState * 2
		then else
			setState * 2 * 0
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
