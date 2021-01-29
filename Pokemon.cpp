#include<iostream>
#include<fstream>

using namespace std;


struct DATA
{
    int value;
    int ID;
};

class Node{
public:
    DATA Data;
    int Size;
    Node *Parent;
    Node *L_child;
    Node *R_child;
    char color;
    Node()
    {
        Data.ID = 0;
        Data.value = 0;
        Size = 1;
        Parent = NULL;
        L_child = NULL;
        R_child = NULL;
        color = 'R';
    }
    Node( int id, int V )
    {
        Data.ID = id;
        Data.value = V;
        Size = 1;
        Parent = NULL;
        L_child = NULL;
        R_child = NULL;
        color = 'R';
    }
    const Node &operator=( const Node x )
    {
        Data = x.Data;
        Size = x.Size;
        Parent = x.Parent;
        L_child = x.L_child;
        R_child = x.R_child;
        return *this;
    }
};

class RBtree
{
public:
    Node *neel;
    Node *root;
    Node *highest;
    Node *lowest;
    RBtree()
    {
        neel = new Node;
        neel->color = 'B';
        neel->Size = 0;
        root = neel;
        root->Parent = neel;
        highest = neel;
        lowest = neel;
    }
    ~RBtree()
    {
        delete neel;
    }
    void LR( Node *x )
    {
        //cout << "enter LR" << endl;
        Node *y = x->R_child;
        x->R_child = y->L_child;
        if( y->L_child != neel ){
            y->L_child->Parent = x;
        }
        y->Parent = x->Parent;
        if( x->Parent == neel ){
            root = y;
        }
        else if( x == x->Parent->L_child ){
            x->Parent->L_child = y;
        }
        else{
            x->Parent->R_child = y;
        }
        y->L_child = x;
        x->Parent = y;
        y->Size = x->Size;
        x->Size = x->L_child->Size + x->R_child->Size + 1;
    }

    void RR( Node *y )
    {
        //cout << "enter RR" << endl;
        Node *x = y->L_child;
        y->L_child = x->R_child;
        if( x->R_child != neel ){
            x->R_child->Parent = y;
        }
        x->Parent = y->Parent;
        if( y->Parent == neel ){
            root = x;
        }
        else if( y == y->Parent->L_child ){
            y->Parent->L_child = x;
        }
        else{
            y->Parent->R_child = x;
        }
        x->R_child = y;
        y->Parent = x;
        x->Size = y->Size;
        y->Size = y->L_child->Size + y->R_child->Size + 1;
    }

    void I_fix( Node *z )
    {
        //cout << "fix1" <<endl;
        //cout << z->Parent->Data.ID << " " << z->Parent->Data.value << " " << z->Parent->color << endl;
        while( z->Parent->color == 'R' ){
            //cout << "fix2" <<endl;
            if( z->Parent == z->Parent->Parent->L_child ){
                Node *uncle = z->Parent->Parent->R_child;
                if( uncle->color == 'R' ){
                    z->Parent->color = 'B';
                    uncle->color = 'B';
                    z->Parent->Parent->color = 'R';
                    z = z->Parent->Parent;
                }
                else{
                    if( z == z->Parent->R_child ){
                        z = z->Parent;
                        LR(z);
                    }
                    z->Parent->color = 'B';
                    z->Parent->Parent->color = 'R';
                    RR(z->Parent->Parent);
                }
            }
            else{
                Node *uncle = z->Parent->Parent->L_child;
                if( uncle->color == 'R' ){
                    z->Parent->color = 'B';
                    uncle->color = 'B';
                    z->Parent->Parent->color = 'R';
                    z = z->Parent->Parent;
                }
                else{
                    if( z == z->Parent->L_child ){
                        z = z->Parent;
                        RR(z);
                    }
                    z->Parent->color = 'B';
                    z->Parent->Parent->color = 'R';
                    LR(z->Parent->Parent);
                }
            }
        }
        root->color = 'B';
    }

    void INSERT( Node *z )
    {
        //cout << " I start " <<endl;
        //cout << z->Data.ID <<endl;
        if( z->Data.value > highest->Data.value ){
            highest = z;
        }
        else if( z->Data.value == highest->Data.value ){
            if( z->Data.ID < highest->Data.ID ){
                highest = z;
            }
        }

        if( z->Data.value < lowest->Data.value ){
            lowest = z;
        }
        else if( z->Data.value == lowest->Data.value ){
            if( z->Data.ID < lowest->Data.ID ){
                lowest = z;
            }
        }

        Node *y = neel;
        Node *x = root;
        //cout << "root:" << root->Data.value << endl;
        while( x != neel ){
            y = x;
            if( z->Data.value < x->Data.value ){
                x = x->L_child;
            }
            else if( z->Data.value == x->Data.value ){
                if( z->Data.ID > x->Data.ID ){
                    x = x->L_child;
                }
                else{
                    x = x->R_child;
                }
            }
            else{
                x = x->R_child;
            }
        }
        //cout << "z's Parent : " << y->Data.ID << " " << y->Data.value << endl;
        z->Parent = y;
        if( y == neel ){
            root = z;
            root->color = 'B';
            highest = z;
            lowest = z;
        }
        else if( z->Data.value < y->Data.value ){
            y->L_child = z;
        }
        else if( z->Data.value == y->Data.value ){
            if( z->Data.ID > y->Data.ID ){
                y->L_child = z;
            }
            else{
                y->R_child = z;
            }
        }
        else{
            y->R_child = z;
        }
        z->L_child = neel;
        z->R_child = neel;
        x = z->Parent;
        while( x != neel ){
            x->Size += 1;
            x = x->Parent;
        }
        if( y != neel ){
            I_fix(z);
        }
        //cout << z->Parent->Data.ID << " " << z->Parent->Data.value << " " << z->Parent->color << endl;

        //cout << "I complete" << endl;
    }

    Node* SEARCH( DATA s )
    {
        Node *ans = root;
        while( ans != neel && ( ans->Data.ID != s.ID || ans->Data.value != s.value ) ){
            if( ans->Data.value > s.value ){
                ans = ans->L_child;
            }
            else if( ans->Data.value < s.value ){
                ans = ans->R_child;
            }
            else{
                if( ans->Data.ID < s.ID ){
                    ans = ans->L_child;
                }
                else{
                    ans = ans->R_child;
                }
            }
        }
        return ans;
    }

    Node* PREDECESSOR( Node *n )
    {
        Node *x = n;
        if( x->R_child != neel ){
            x = x->R_child;
            while( x->L_child != neel ){
                x = x->L_child;
            }
            return x;
        }
        Node*y = n->Parent;
        while( y != neel && x == y->R_child ){
            x = y;
            y = y->Parent;
        }
        return y;
    }

    Node* SUCCESSOR( Node *n )
    {
        Node *x = n;
        if( x->L_child != neel ){
            x = x->L_child;
            while( x->R_child != neel ){
                x = x->R_child;
            }
            return x;
        }
        Node*y = n->Parent;
        while( y != neel && x == y->L_child ){
            x = y;
            y = y->Parent;
        }
        return y;
    }

    void D_fix( Node *x ){
        while( x != root && x->color == 'B' ){
            if( x == x->Parent->L_child ){
                Node *sibling = x->Parent->R_child;
                if( sibling->color == 'R' ){
                    sibling->color = 'B';
                    x->Parent->color = 'R';
                    LR(x->Parent);
                    sibling = x->Parent->R_child;
                }
                if( sibling->L_child->color == 'B' && sibling->R_child->color == 'B' ){
                    sibling->color = 'R';
                    x = x->Parent;
                }
                else{
                    if( sibling->R_child->color == 'B' ){
                        sibling->L_child->color = 'B';
                        sibling->color = 'R';
                        RR(sibling);
                        sibling = x->Parent->R_child;
                    }
                    sibling->color = x->Parent->color;
                    x->Parent->color = 'B';
                    sibling->R_child->color = 'B';
                    LR(x->Parent);
                    x = root;
                }
            }
            else{
                Node *sibling = x->Parent->L_child;
                if( sibling->color == 'R' ){
                    sibling->color = 'B';
                    x->Parent->color = 'R';
                    RR(x->Parent);
                    sibling = x->Parent->L_child;
                }
                if( sibling->L_child->color == 'B' && sibling->R_child->color == 'B' ){
                    sibling->color = 'R';
                    x = x->Parent;
                }
                else{
                    if( sibling->L_child->color == 'B' ){
                        sibling->R_child->color = 'B';
                        sibling->color = 'R';
                        LR(sibling);
                        sibling = x->Parent->L_child;
                    }
                    sibling->color = x->Parent->color;
                    x->Parent->color = 'B';
                    sibling->L_child->color = 'B';
                    RR(x->Parent);
                    x = root;
                }
            }
        }
        x->color = 'B';
    }

    Node* DELETE( DATA D )
    {
        Node *z = SEARCH(D);
        Node *temp_lowest;
        //cout << "z-- " << "ID:" << z->Data.ID << "  value:" << z->Data.value << "  color:" << z->color << "  size:" << z->Size << "  Parent:" <<  z->Parent->Data.value << endl;
        Node *y = neel;
        if( z != neel ){
            if( D.ID == highest->Data.ID && D.value == highest->Data.value ){
                highest = SUCCESSOR(highest);
            }
            if( D.ID == lowest->Data.ID && D.value == lowest->Data.value ){
                temp_lowest = SUCCESSOR(lowest);
                if( temp_lowest != neel ){
                    lowest = temp_lowest;
                }
                else{
                    lowest = PREDECESSOR(lowest);
                }
            }
            Node *x;
            if( z->L_child == neel || z->R_child == neel ){
                y = z;
            }
            else{
                y = SUCCESSOR(z);
                //cout << "SUCCESSOR : " << y->Data.value << endl;
            }
            if( y->L_child != neel ){
                x = y->L_child;
            }
            else{
                x = y->R_child;
            }
            x->Parent = y->Parent;
            if( y->Parent == neel ){
                root = x;
                root->color = 'B';
            }
            else if( y == y->Parent->L_child ){
                y->Parent->L_child = x;
            }
            else{
                y->Parent->R_child = x;
            }
            if( y != z ){
                //cout << "y != z" << endl;
                z->Data.ID = y->Data.ID;
                z->Data.value = y->Data.value;
            }
            z = y->Parent;
            while( z != neel ){
                z->Size -= 1;
                z = z->Parent;
            }
            if (y->color == 'B') {
                D_fix(x);
            }
        }
        return y;
    }

    void TREE_WALK( Node *x )
    {
        if( x != neel ){
            TREE_WALK( x->L_child );
            cout << "ID:" << x->Data.ID << "  value:" << x->Data.value << "  color:" << x->color << "  size:" << x->Size << "  Parent:" <<  x->Parent->Data.ID << endl;
            TREE_WALK( x->R_child );
        }
    }

    int GET_RANK( Node *x )
    {
        int ans = 0;
        if( x == neel ){
            return ans;
        }
        ans = x->R_child->Size + 1;
        Node *y = x;
        while( y != root ){
            if( y == y->Parent->L_child ){
                ans = ans + y->Parent->R_child->Size + 1;
            }
            y = y->Parent;
        }
        return ans;
    }

    Node* RANK_NODE( int Rank )
    {
        Node *x = root;
        while( x != neel ){
            int y = GET_RANK(x);
            if( y == Rank ){
                return x;
            }
            else if( y < Rank ){
                x = x->L_child;
            }
            else{
                x = x->R_child;
            }
        }
    }

    int V_rank( int V )
    {
        Node *ans = root;
        while(1){
            if( ans->Data.value > V ){
                ans = ans->L_child;
            }
            else if( ans->Data.value < V ){
                ans = ans->R_child;
            }
            else{
                Node *temp_ans = ans->R_child;
                while( temp_ans != neel ){
                    if( temp_ans->Data.value == V ){
                        ans = temp_ans;
                        temp_ans = temp_ans->R_child;
                    }
                    else{
                        temp_ans = temp_ans->L_child;
                    }
                }
                return GET_RANK(ans);
            }
        }
    }

    int ModeB_HIGH( int high, int low )
    {
        if( high >= highest->Data.value ){
            return GET_RANK(highest);
        }
        else{
            //cout << "----------ModeB_HIGH----------" << endl;
            Node *high_ans = root;
            Node *ans = root;
            //cout << "BBBBBBBBBB" << endl;
            while( ans != neel ){
                if( ans->Data.value > high ){
                    ans = ans->L_child;
                }
                else{
                    high_ans = ans;
                    ans = ans->R_child;
                }
                //cout << "ans :" << ans->Data.value << endl;
            }
            //cout << "high_ans_value: " << high_ans->Data.value << endl;
            if( high_ans->Data.value < low ){
                return 0;
            }
            else{
                return GET_RANK(high_ans);
            }
        }
    }

    int ModeB_LOW( int low )
    {
        if( low <= lowest->Data.value ){
            return GET_RANK(lowest);
        }
        else{
            //cout << "----------ModeB_LOW----------" << endl;
            Node *low_ans = root;
            Node *ans = root;
            int V;
            while( ans != neel ){
                if( ans->Data.value < low ){
                    ans = ans->R_child;
                }
                /*else if( ans->Data.value == low ){
                    low_ans = ans;
                    ans = ans->R_child;
                    while( ans != neel ){
                        if( ans->Data.value == low_ans->Data.value ){
                            low_ans = ans;
                            ans = ans->R_child;
                        }
                        else{
                            ans = ans->L_child;
                        }
                    }
                }*/
                else{
                    if( ans->Data.value != low_ans->Data.value ){
                        low_ans = ans;
                    }
                    ans = ans->L_child;
                }

                //cout << "ans :" << ans->Data.value << endl;
                //cout << "low_ans :" << low_ans->Data.value << endl;
            }
            V = low_ans->Data.value;
            ans = low_ans->R_child;
            while( ans != neel ){
                if( ans->Data.value == V ){
                    low_ans = ans;
                    ans = ans->R_child;
                }
                else{
                    ans = ans->L_child;
                }
            }
            return GET_RANK(low_ans);
        }
    }

    void ModeA( int best, int worst, int *A_ans )
    {
        Node *ans_best;
        Node *ans_worst;
        Node *temp_worst;
        Node *temp_best;
        Node *test_best;
        Node *test_worst;
        Node *pre_best;
        Node *half_rank;
        int best_pre_v;
        int lowest_rank;
        int temp_rank;
        int dist;
        if( best <= 1 ){
            *(A_ans) = highest->Data.ID;
            *(A_ans+1) = highest->Data.value;
            if( worst >= root->Size ){
                *(A_ans+2) = lowest->Data.ID;
                *(A_ans+3) = lowest->Data.value;
            }
            else{
                ans_worst = RANK_NODE(worst);
                if( ans_worst->Data.value == highest->Data.value ){
                    *(A_ans+2) = highest->Data.ID;
                    *(A_ans+3) = highest->Data.value;
                }
                else{
                    temp_worst = ans_worst;
                    dist = 1;
                    temp_rank = GET_RANK(temp_worst);
                    while( temp_rank - dist > 500 ){
                        half_rank = RANK_NODE( temp_rank - (temp_rank-dist/2) );
                        if( temp_worst->Data.value == half_rank->Data.value ){
                            temp_worst = half_rank;
                            temp_rank = temp_rank - (temp_rank-dist/2);
                        }
                        else{
                            dist = temp_rank - (temp_rank-dist/2);
                        }
                    }
                    test_worst = PREDECESSOR(temp_worst);
                    while(1){
                        if( temp_worst->Data.value != test_worst->Data.value ){
                            break;
                        }
                        else{
                            temp_worst = test_worst;
                            test_worst = PREDECESSOR(temp_worst);
                        }
                    }
                    *(A_ans+2) = temp_worst->Data.ID;
                    *(A_ans+3) = temp_worst->Data.value;
                }
            }
        }
        else{
            ans_best = RANK_NODE(best);
            ans_worst = RANK_NODE(worst);
            pre_best = PREDECESSOR(ans_best);
            best_pre_v = pre_best->Data.value;
            if( worst >= root->Size ){ //----------- 1 > best && root_size >= worst ------------------------
                lowest_rank = GET_RANK(lowest);
                if( lowest_rank > best ){
                    *(A_ans+2) = lowest->Data.ID;
                    *(A_ans+3) = lowest->Data.value;
                    if( ans_best->Data.value != best_pre_v ){
                        *(A_ans) = ans_best->Data.ID;
                        *(A_ans+1) = ans_best->Data.value;
                    }
                    else{
                        if( ans_best->Data.value > lowest->Data.value ){
                            temp_best = ans_best;
                            dist = GET_RANK(lowest);
                            temp_rank = GET_RANK(temp_best);
                            while( dist - temp_rank > 500 ){
                                half_rank = RANK_NODE( dist - (dist-temp_rank/2) );
                                if( temp_best->Data.value == half_rank->Data.value ){
                                    temp_best = half_rank;
                                    temp_rank = dist - (dist-temp_rank/2);
                                }
                                else{
                                    dist = dist - (dist-temp_rank/2);
                                }
                            }
                            test_best = SUCCESSOR(temp_best);
                            while(1){
                                if( test_best->Data.value != temp_best->Data.value ){
                                    break;
                                }
                                else{
                                    temp_best = test_best;
                                    test_best = SUCCESSOR(temp_best);
                                }
                            }
                            *(A_ans) = test_best->Data.ID;
                            *(A_ans+1) = test_best->Data.value;
                        }
                        else{
                            *(A_ans) = 0;
                            *(A_ans+1) = 0;
                            *(A_ans+2) = 0;
                            *(A_ans+3) = 0;
                        }
                    }
                }
                else if( lowest_rank == best ){
                    *(A_ans) = lowest->Data.ID;
                    *(A_ans+1) = lowest->Data.value;
                    *(A_ans+2) = lowest->Data.ID;
                    *(A_ans+3) = lowest->Data.value;
                }
                else{
                    *(A_ans) = 0;
                    *(A_ans+1) = 0;
                    *(A_ans+2) = 0;
                    *(A_ans+3) = 0;
                }
            }
            else{  //------- 1 > best && worst < root_size -----------
                temp_worst = ans_worst;
                test_worst = PREDECESSOR(ans_worst);
                if( ans_best->Data.value != pre_best->Data.value ){
                    *(A_ans) = ans_best->Data.ID;
                    *(A_ans+1) = ans_best->Data.value;
                    if( ans_worst->Data.value == ans_best->Data.value ){
                        *(A_ans+2) = ans_best->Data.ID;
                        *(A_ans+3) = ans_best->Data.value;
                    }
                    else{
                        dist = GET_RANK(ans_best);
                        temp_rank = GET_RANK(temp_worst);
                        while( temp_rank - dist > 500 ){
                            half_rank = RANK_NODE( temp_rank - (temp_rank-dist/2) );
                            if( temp_worst->Data.value == half_rank->Data.value ){
                                temp_worst = half_rank;
                                temp_rank = temp_rank - (temp_rank-dist/2);
                            }
                            else{
                                dist = temp_rank - (temp_rank-dist/2);
                            }
                        }
                        test_worst = PREDECESSOR(temp_worst);
                        while(1){
                            if( temp_worst->Data.value != test_worst->Data.value ){
                                break;
                            }
                            else{
                                temp_worst = test_worst;
                                test_worst = PREDECESSOR(temp_worst);
                            }
                        }
                        *(A_ans+2) = temp_worst->Data.ID;
                        *(A_ans+3) = temp_worst->Data.value;
                    }
                }
                else{
                    if( ans_best->Data.value > ans_worst->Data.value ){
                        temp_best = ans_best;
                        dist = GET_RANK(ans_worst);
                        temp_rank = GET_RANK(temp_best);
                        while( dist - temp_rank > 500 ){
                            half_rank = RANK_NODE( dist - (dist-temp_rank/2) );
                            if( temp_best->Data.value == half_rank->Data.value ){
                                temp_best = half_rank;
                                temp_rank = dist - (dist-temp_rank/2);
                            }
                            else{
                                dist = dist - (dist-temp_rank/2);
                            }
                        }
                        test_best = SUCCESSOR(temp_best);
                        while(1){
                            if( test_best->Data.value != temp_best->Data.value ){
                                break;
                            }
                            else{
                                temp_best = test_best;
                                test_best = SUCCESSOR(temp_best);
                            }
                        }
                        *(A_ans) = test_best->Data.ID;
                        *(A_ans+1) = test_best->Data.value;
                        if( ans_worst->Data.value == test_best->Data.value ){
                            *(A_ans+2) = test_best->Data.ID;
                            *(A_ans+3) = test_best->Data.value;
                        }
                        else{
                            dist = GET_RANK(test_best);
                            temp_rank = GET_RANK(temp_worst);
                            while( temp_rank - dist > 500 ){
                                half_rank = RANK_NODE( temp_rank - (temp_rank-dist/2) );
                                if( temp_worst->Data.value == half_rank->Data.value ){
                                    temp_worst = half_rank;
                                    temp_rank = temp_rank - (temp_rank-dist/2);
                                }
                                else{
                                    dist = temp_rank - (temp_rank-dist/2);
                                }
                            }
                            test_worst = PREDECESSOR(temp_worst);
                            while(1){
                                if( temp_worst->Data.value != test_worst->Data.value ){
                                    break;
                                }
                                else{
                                    temp_worst = test_worst;
                                    test_worst = PREDECESSOR(temp_worst);
                                }
                            }
                            *(A_ans+2) = temp_worst->Data.ID;
                            *(A_ans+3) = temp_worst->Data.value;
                        }
                    }
                    else{
                        *(A_ans) = 0;
                        *(A_ans+1) = 0;
                        *(A_ans+2) = 0;
                        *(A_ans+3) = 0;
                    }
                }
            }
        }
    }

};


int main(int argc, char *argv[])
{
    ifstream input(argv[1]);
    ofstream output(argv[2]);
    RBtree T;
    char Mode;
    int id;
    int v;
    int Rank;
    int Value;
    int V1, V2, high, low, high_ans, low_ans;
    int R1, R2, best, worst;
    int A_ans[4] = {0};
    while( input >> Mode ){
        switch( Mode ){
            case 'I':
                {
                    input >> id >> v;
                    Node *N = new Node(id, v);
                    T.INSERT(N);
                }
                break;
            case 'D':
                {
                    input >> id >> v;
                    DATA D;
                    D.ID = id;
                    D.value = v;
                    Node* d_node;
                    d_node = T.DELETE(D);
                    if( d_node != T.neel ){
                        delete d_node;
                        d_node = NULL;
                    }
                }
                break;
            case 'R':
                {
                    input >> Rank;
                    DATA Rans = T.RANK_NODE(Rank)->Data;
                    output << "R " << Rans.ID << " " << Rans.value << endl;
                }
                break;
            case 'V':
                {
                    input >> Value;
                    output << "V " << T.V_rank(Value) << endl;
                }
                break;
            case 'B':
                {
                    input >> V1 >> V2;
                    if( V1 < V2 ){
                        high = V2;
                        low = V1;
                    }
                    else{
                        high = V1;
                        low = V2;
                    }
                    if( high < T.lowest->Data.value ){
                        output << "B 0" << endl;
                    }
                    else if( low > T.highest->Data.value ){
                        output << "B 0" << endl;
                    }
                    /*else if( high == T.lowest->Data.value ){
                        output << "B " << T.root->Size << " " << T.root->Size << endl;
                    }*/
                    else if( low == T.highest->Data.value ){
                        output << "B 1 1" << endl;
                    }
                    else{
                        high_ans = T.ModeB_HIGH(high, low);
                        if( high_ans == 0 ){
                            output << "B 0" << endl;
                        }
                        else{
                            low_ans = T.ModeB_LOW(low);
                            output << "B " << high_ans << " " << low_ans << endl;
                        }
                    }
                }
                break;
            case 'A':
                {
                    input >> R1 >> R2;
                    if( R1 < R2 ){
                        worst = R2;
                        best = R1;
                    }
                    else{
                        worst = R1;
                        best = R2;
                    }
                    if( worst == 0 ){
                        output << "A 0" << endl;
                    }
                    else if( best > T.root->Size ){
                        output << "A 0" << endl;
                    }
                    else if( worst == 1 ){
                        output << "A " << T.highest->Data.ID << " " << T.highest->Data.value << " " << T.highest->Data.ID << " " << T.highest->Data.value << endl;
                    }
                    else{
                        T.ModeA(best, worst, A_ans);
                        if( ( A_ans[0] == 0 && A_ans[1] == 0 ) || ( A_ans[2] == 0 && A_ans[3] == 0 ) ){
                            output << "A 0" << endl;
                        }
                        else{
                            output << "A " << A_ans[0] << " " << A_ans[1] << " " << A_ans[2] << " " << A_ans[3] << endl;
                        }
                    }
                }
                break;
            default:
                break;
        }
    }
    input.close();
    output.close();
    return 0;
}







