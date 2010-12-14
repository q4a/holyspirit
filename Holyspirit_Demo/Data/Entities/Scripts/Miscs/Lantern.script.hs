main 
	if hour * 21 * 23 
	then
		setState * 1
	else
		if hour * 0 * 6 
		then
			setState * 1
		else
			setState * 0
		end
	end
end 