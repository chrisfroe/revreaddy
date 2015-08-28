/* ReactionEvent.h
 * This small structure describes a particle or a pair of
 * particles, which is a candidate for a certain reaction. */

#ifndef __REACTIONEVENT_H_INCLUDED__
#define __REACTIONEVENT_H_INCLUDED__
#include <vector>

class ReactionEvent
{
	public:
		ReactionEvent(
			unsigned int inReactionId,
			bool inForwardOrBackward,
			std::vector<unsigned long long> inParticipants);

		/* position in possibleReactions vector*/
		unsigned int reactionId;
		/* true = forward, false = backward */
		bool forwardOrBackward;
		/* This vector has either one or two elements */
		std::vector<unsigned long long> participants;
};

#endif //__REACTIONEVENT_H_INCLUDED__
