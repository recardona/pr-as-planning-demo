/*
Plan Recognition as Planning Demo: Home domain
Copyright (C) 2012
Miquel Ramirez <miquel.ramirez@rmit.edu.au>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <model/PickUp.hxx>
#include <model/Room.hxx>
#include <model/Agent.hxx>
#include <model/Item.hxx>

namespace Application
{

PickUp::PickUp( Agent* a, Room* r, Item* i )
	: mAgent(a), mRoom(r), mItem(i), mSTRIPSAction(NULL)
{
}

PickUp::~PickUp()
{
}

const QString&	PickUp::roomName() const
{
	return mRoom->name();
}

const QString&	PickUp::itemName() const
{
	return mItem->name();
}

void	PickUp::execute()
{
	mRoom->removeItem( mItem );
	mAgent->addItem( mItem );
	assert( mSTRIPSAction != NULL );
	emit executed( mSTRIPSAction->index() );
}

bool	PickUp::checkPrecondition()
{
	return mRoom->isAgentHere() && mRoom->contents().contains( mItem );
}

void	PickUp::makeSTRIPSAction( STRIPS_Problem& p )
{
	assert( mRoom->atFluent() != NULL );
	assert( mRoom->atItemFluents()[ mItem->name() ] != NULL );
	assert( mAgent->carryingFluents()[ mItem->name() ] );
	
	std::string signature = QString( "(PickUp %1 %2)" )
				.arg( mItem->name() ).arg( mRoom->name() ).toStdString();

	mPrecFluents.push_back( mRoom->atFluent() );
	mPrecFluents.push_back( mRoom->atItemFluents()[ mItem->name() ] );
	mAddFluents.push_back( mAgent->carryingFluents()[ mItem->name() ] );
	mDelFluents.push_back( mRoom->atItemFluents()[ mItem->name() ] );

	aptk::Fluent_Vec pre, add, del;
	for (unsigned k = 0; k < mPrecFluents.size(); k++ )
		pre.push_back( mPrecFluents[k]->index() );
	for (unsigned k = 0; k < mAddFluents.size(); k++ )
		add.push_back( mAddFluents[k]->index() );
	for (unsigned k = 0; k < mDelFluents.size(); k++ )
		del.push_back( mDelFluents[k]->index() );

	assert( !pre.empty() );
	assert( !add.empty() );
	assert( !del.empty() );
	aptk::Conditional_Effect_Vec cEffs;

	unsigned actIndex = STRIPS_Problem::add_action( p, signature, pre, add, del, cEffs ); 
	mSTRIPSAction = p.actions()[actIndex];
	assert( !mSTRIPSAction->prec_vec().empty() );
	assert( !mSTRIPSAction->add_vec().empty() );
	assert( !mSTRIPSAction->del_vec().empty() );
	
}

}
