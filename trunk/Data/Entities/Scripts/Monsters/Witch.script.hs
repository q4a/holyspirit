
main 
		variable * 0 * 1
	
	if variable * 0 * 1
	then
		setFriendly * 0
		stop_speak
	
		if alive
		then
			if see
			then
				if distance * 4
				then
					if distance * 2
					then
						useMiracle * 0
					else
						fight
					end
				else
					useMiracle * 1
				end
			else
				randomDisplace
			end
		else
			if variable * 1 * 0
			then
				stopMiracle * 0
				stopMiracle * 1
				
				useMiracle * 2
				
				if miracle * 2
				then
				variable * 1 * 1
				end
			end
		end
	end
end 
