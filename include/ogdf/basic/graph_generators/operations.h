/** \file
 * \brief Declaration of graph operations
 *
 * \author Max Ilsen
 *
 * \par License:
 * This file is part of the Open Graph Drawing Framework (OGDF).
 *
 * \par
 * Copyright (C)<br>
 * See README.md in the OGDF root directory for details.
 *
 * \par
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * Version 2 or 3 as published by the Free Software Foundation;
 * see the file LICENSE.txt included in the packaging of this file
 * for details.
 *
 * \par
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * \par
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, see
 * http://www.gnu.org/copyleft/gpl.html
 */

#pragma once

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/NodeArray.h>

namespace ogdf {

/**
 * @addtogroup graph-generators
 * @{
 */

//! @name Graph operations
//! @{

/**
 * Forms the disjoint union of \p G1 and \p G2.
 *
 * @param G1 is the first graph and assigned the graph union.
 * @param G2 is the second graph.
 */
inline void graphUnion(Graph& G1, const Graph& G2) { G1.insert(G2); }

/**
 * Forms the union of \p G1 and \p G2 while identifying nodes from \p G2 with
 * nodes from \p G1.
 *
 * @param G1 is the first graph and assigned the graph union.
 * @param G2 is the second graph.
 * @param map2to1 identifies nodes from \p G2 with nodes from \p G1.
 * Empty entries in \p map2to1 have to be \c nullptr.
 * It is assigned a mapping from nodes in \p G2 to the union \p G1.
 * @param parallelfree sets whether the resulting graph union should not contain
 * multi-edges.
 * @param directed sets whether the graph union is treated as directed or
 * undirected when detecting multi-edges. It only has an effect if
 * \p parallelfree is set.
 */
OGDF_EXPORT void graphUnion(Graph& G1, const Graph& G2, NodeArray<node>& map2to1,
		bool parallelfree = false, bool directed = false);

using NodeMap = NodeArray<NodeArray<node>>;

/**
 * Computes the graph product of \p G1 and \p G2, using a given function to add
 * edges.
 *
 * First, \p product is cleared. \f$|V(G1)|\cdot|V(G2)|\f$ nodes are added to it
 * and \p addEdges is called for each pair of nodes in \f$V(G1) \times V(G2)\f$.
 *
 * @param G1 is the first input graph.
 * @param G2 is the second input graph.
 * @param product is assigned the graph product.
 * @param nodeInProduct is assigned a mapping from nodes of (\p G1, \p G2) to \p product.
 * @param addEdges A function that adds edges to the graph product for each pair
 * of nodes in \f$V(G1) \times V(G2)\f$.
 */
OGDF_EXPORT void graphProduct(const Graph& G1, const Graph& G2, Graph& product,
		NodeMap& nodeInProduct, const std::function<void(node, node)>& addEdges);

/**
 * Computes the Cartesian product of \p G1 and \p G2 and assigns it to \p product,
 * with \f$E =
 * 		\{(\langle v_1,w_1\rangle, \langle v_1,w_2\rangle) |
 * 			(w_1,w_2) \in E_2\} \cup
 * 		\{(\langle v_1,w_1\rangle, \langle v_2,w_1\rangle) |
 * 			(v_1,v_2) \in E_1\}
 * \f$.
 *
 * Multi-edges are kept and incorporated into the graph product.
 *
 * @param G1 is the first input graph.
 * @param G2 is the second input graph.
 * @param product is assigned the graph product.
 * @param nodeInProduct is assigned a mapping from nodes of (\p G1, \p G2) to \p product.
 */
OGDF_EXPORT void cartesianProduct(const Graph& G1, const Graph& G2, Graph& product,
		NodeMap& nodeInProduct);

/**
 * Computes the tensor product of \p G1 and \p G2 and assigns it to \p product,
 * with \f$E =
 * 		\{(\langle v_1,w_1\rangle, \langle v_2,w_2\rangle) |
 * 			(v_1,v_2) \in E_1 \land (w_1,w_2) \in E_2\}
 * \f$.
 *
 * @copydetails cartesianProduct(const Graph&, const Graph&, Graph&, NodeMap&)
 */
OGDF_EXPORT void tensorProduct(const Graph& G1, const Graph& G2, Graph& product,
		NodeMap& nodeInProduct);

/**
 * Computes the lexicographical product of \p G1 and \p G2 and assigns it to \p product,
 * with \f$E =
 * 		\{(\langle v_1,w_1\rangle, \langle v_2,w_2\rangle) |
 * 			(v_1,v_2) \in E_1\} \cup
 * 		\{(\langle v_1,w_1\rangle, \langle v_1,w_2\rangle) |
 * 			(w_1,w_2) \in E_2\}
 * \f$.
 *
 * @warning The lexicographical product is not commutative!
 * @copydetails cartesianProduct(const Graph&, const Graph&, Graph&, NodeMap&)
 */
OGDF_EXPORT void lexicographicalProduct(const Graph& G1, const Graph& G2, Graph& product,
		NodeMap& nodeInProduct);

/**
 * Computes the strong product of \p G1 and \p G2 and assigns it to \p product,
 * with \f$E =
 * 		\{(\langle v_1,w_1\rangle, \langle v_1,w_2\rangle) |
 * 			(w_1,w_2) \in E_2\} \cup
 * 		\{(\langle v_1,w_1\rangle, \langle v_2,w_1\rangle) |
 * 			(v_1,v_2) \in E_1\} \cup
 * 		\{(\langle v_1,w_1\rangle, \langle v_2,w_2\rangle) |
 * 			(v_1,v_2) \in E_1 \land (w_1,w_2) \in E_2\}
 * \f$.
 *
 * @copydetails cartesianProduct(const Graph&, const Graph&, Graph&, NodeMap&)
 */
OGDF_EXPORT void strongProduct(const Graph& G1, const Graph& G2, Graph& product,
		NodeMap& nodeInProduct);

/**
 * Computes the co-normal product of \p G1 and \p G2 and assigns it to \p product,
 * with \f$E =
 * 		\{(\langle v_1,w_1\rangle, \langle v_2,w_2\rangle) |
 * 			(v_1,v_2) \in E_1 \lor (w_1,w_2) \in E_2\}
 * \f$.
 *
 * @copydetails cartesianProduct(const Graph&, const Graph&, Graph&, NodeMap&)
 */
OGDF_EXPORT void coNormalProduct(const Graph& G1, const Graph& G2, Graph& product,
		NodeMap& nodeInProduct);

/**
 * Computes the modular product of \p G1 and \p G2 and assigns it to \p product,
 * with \f$E =
 * 		\{(\langle v_1,w_1\rangle, \langle v_2,w_2\rangle) |
 * 			(v_1,v_2) \in E_1 \land (w_1,w_2) \in E_2\} \cup
 * 		\{(\langle v_1,w_1\rangle, \langle v_2,w_2\rangle) |
 * 			(v_1,v_2) \not\in E_1 \land (w_1,w_2) \not\in E_2\}
 * \f$.
 *
 * @copydetails cartesianProduct(const Graph&, const Graph&, Graph&, NodeMap&)
 */
OGDF_EXPORT void modularProduct(const Graph& G1, const Graph& G2, Graph& product,
		NodeMap& nodeInProduct);

/**
 * Computes the rooted product of \p G1 and \p G2, rooted in \p rootInG2,
 * and assigns it to \p product.
 *
 * @copydetails cartesianProduct(const Graph&, const Graph&, Graph&, NodeMap&)
 * @param rootInG2 is the node of \p G2 that is identified with every node of
 * \p G1 once in order to create the rooted product.
 */
OGDF_EXPORT void rootedProduct(const Graph& G1, const Graph& G2, Graph& product,
		NodeMap& nodeInProduct, node rootInG2);

/**
 * Computes the complement of G.
 *
 * @param G is the input graph, the complement will be assigned to G.
 * @param directional Whether directionality should be considered when computing the complement graph. (default is false)
 * @param allow_self_loops Whether to allow self loops, if false and G contains self loops, these will not be removed. (default is false)
 */
OGDF_EXPORT void complement(Graph& G, bool directional = false, bool allow_self_loops = false);
/**
 * Computes the intersection of G1 and G2. The output will be assigned to G1.
 *
 * @param G1 is the first graph, the intersection will be assigned to G1.
 * @param G2 is the second graph.
 * @param nodeMap associates a node in G2 with a node in G1. There should be an entry for every node in G1, this entry may be a nullptr.
 */
OGDF_EXPORT void intersection(Graph& G1, const Graph& G2, const NodeArray<node>& nodeMap);

/**
 * Computes the joined graph of G1 and G2. The output will be assigned to G1.
 * \f$ (V = V_1 \cup V_2, E = E_1 \cup E_2 \cup V_1 \cross V_2) \f$
 *
 * @param G1 is the first graph, the joined graph will be assigned to G1.
 * @param G2 is the second graph.
 * @param nodeMap is assigned a mapping from nodes in \p G2 to new nodes in G1. It should be initialized like this: NodeArray<node> nodeMap(G2);
 */
OGDF_EXPORT void join(Graph& G1, const Graph& G2, NodeArray<node>& nodeMap);


//! @}

/** @} */
}
