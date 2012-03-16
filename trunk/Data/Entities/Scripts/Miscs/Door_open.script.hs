
main 
	if getState * 1
	then
		setActif * 0
		setCollision * 0
	end

	if interact
	then
		stop_interact
		setActif * 0
		setState * 1 * 0
		setCollision * 0
	end
end 
