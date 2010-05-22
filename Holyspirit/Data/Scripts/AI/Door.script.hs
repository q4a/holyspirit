
main 
	if talk
	then
		if variable * 0 * 1
		then
			variable * 0 * 0
			stop_talk
		else
			variable * 0 * 1
			stop_talk
		end
	end

	if variable * 1 * 0
	then
		if variable * 0 * 1
		then
			setState * 3
			setCollision * 0
		else
			setState * 0
			setCollision * 1
		end
	else
		setState * 0
		setCollision * 1
	end
end 
