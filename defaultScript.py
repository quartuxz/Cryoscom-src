import math

def rotate(origin, point, angle):
    """
    Rotate a point counterclockwise by a given angle around a given origin.

    The angle should be given in radians.
    """
    ox, oy = origin
    px, py = point

    qx = ox + math.cos(angle) * (px - ox) - math.sin(angle) * (py - oy)
    qy = oy + math.sin(angle) * (px - ox) + math.cos(angle) * (py - oy)
    return qx, qy



def AIBehaviour(playerPos, playerRadius, xPlayerVel, yPlayerVel,
                canFire,
                xThisPos, yThisPos, thisRadius, bulletSpeed,
                hasHead, xHeadPos, yHeadPos, headRadius, xDefaultDistance, yDefaultDistance):
    #origin = (xThisPos, yThisPos)
    #point = (playerPos[0][0], playerPos[0][1])
    rx, ry = playerPos[0][0], playerPos[0][1]
    #rx, ry = rotate(origin, point, math.pi)
    distanceToPlayer = math.sqrt(math.pow(playerPos[0][0]-xThisPos,2)+math.pow(playerPos[0][1]+yThisPos,2))
    #how many times the starting velocity of the player is multiplied by
    velocityMultiplier = 2
    if distanceToPlayer<(200+playerRadius+thisRadius):
        velocityMultiplier=2

    return(rx, ry, velocityMultiplier, playerPos[0][0], playerPos[0][1],False)
