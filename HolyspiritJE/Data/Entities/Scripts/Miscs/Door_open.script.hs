
main 
	if getState * 1
	then
		setActif * 0
		setCollision * 0
	end

	if talk
	then
		stop_talk
		setActif * 0
		setState * 1 * 0
		setCollision * 0
	end
end 
