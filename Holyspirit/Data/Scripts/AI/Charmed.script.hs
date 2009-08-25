
main
	if alive
	then
		if distance * 9
		then
			if distance * 7 
			then
				if see
				then
					if shooter
					then
						shoot
					else
						fight
					end
				else
					if distance * 2
					then
						randomDisplace
					else
						followHero
					end
				end
			else
				followHero
			end
		else
			followHero
			teleport
		end
	end
end
