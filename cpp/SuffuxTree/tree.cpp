#include "tree.h"


namespace NSufixTree {

TEdge* TEdge::split(size_t position) {
    //TSubstring upPart(subString);
    TSubstring downPart(subString);
    //upPart.end = position;
    downPart.start = position;
    subString.end = position;

    std::unique_ptr<TNode> newNode(new TNode(this));
    newNode->addEdge(downPart);
    newNode->get(downPart.head())->endNode.swap(endNode);
    endNode.swap(newNode);
    return endNode->get(downPart.head());
}

void TTreeBase::ukkonenRebuildTree() {
    for (size_t i = 0; i < text.size(); ++i) {
        std::cerr << "ukkonenPush i: " << i << " ch: " << text[i] << "\n";
        ukkonenPush(i);
        std::cerr << "\n\n";
    }
}

void TTreeBase::ukkonenPush(size_t position) {
    cursors.emplace_back(TTreeCursor(&rootEdge));
    TNode* nodeForLink = nullptr;

    for (auto& cursor : cursors) {
        int stepType = cursor.step(position);
        std::cerr << "step: " << stepType << '\n';

        if (stepType > 1) {
            std::cerr << "link\n";
            TNode* currentNode = cursor.edge->parentNode;
            if (nodeForLink != nullptr && nodeForLink->suffixLink == nullptr) {
                nodeForLink->suffixLink = currentNode;
            }
            nodeForLink = currentNode;
        }
        // if (isThirdType) {
        //     return;
        // }
        // std::cerr << "pop\n";
        // cursors.pop_front();
    }
}

int TTreeCursor::step(size_t position) {
    std::cerr << "  pos: " << position << std::endl;
    char ch = edge->subString.str[position];
    std::cerr << "  ch: " << ch << std::endl;

    if (edge->subString.positionIsValid(cursor)) {
        std::cerr << "position is valid" << std::endl;
        if (edge->subString.str[cursor] == ch) {
            //! just follow
            std::cerr << "just follow" << std::endl;
            ++cursor;
            return 1;
        } else {
            edge = edge->split(cursor);
            return 2;
        }
    } else {
        if (edge->endNode == nullptr) {
            //! create node at the end
            // throw TSimpleException() << "WTF!?";
            std::cerr << "create end node" << std::endl;
            edge->endNode.reset(
                new TNode(edge)
            );
        }

        if (edge->endNode->has(ch)) {
            edge = edge->endNode->get(ch);
            std::cerr << "Yes! Third type" << std::endl;
            return 3; // Yes! Third type of step
        } else {
            edge->endNode->addEdge(
                TSubstring(edge->subString.str, position)
            );
            edge = edge->endNode->get(ch);
            return 2;
        }
    }
    return 0;
}

void TEdge::show(size_t lvl, std::ostream& os) const {
    size_t end = subString.end;
    if (end > subString.str.size()) {
        end = subString.str.size();
    }
    os << "[" << subString.start << " : " << end << "]" << subString.copy() << std::endl;
    //*dbg*/ std::cerr << "edge show\n";
    if (endNode != nullptr) {
        endNode->show(lvl + 1, os);
    }
    //*dbg*/ std::cerr << "edge show end\n";
}

void TNode::show(size_t lvl, std::ostream& os) const {
    //*dbg*/ std::cerr << "node show\n";
    ++lvl;
    for (size_t i = 0; i < edges.size(); ++i) {
        if (nullptr != edges[i]) {
            os  << std::string(lvl, ' ')
                << "-("
                << static_cast<char>(FIRST_ABC_CHAR + i)
                << "): "
            ;
            edges[i]->show(lvl, os);
            // os << '\n';
        } else {
            // os << "-";
            // os << '\n';
        }
    }
}

void TTreeBase::show(std::ostream& os) const {
    rootEdge.show(0, os);
}

}

