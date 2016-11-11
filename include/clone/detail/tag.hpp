/// \file
/// \brief The tag header

#ifndef CLONE_DETAIL_TAG_H
#define CLONE_DETAIL_TAG_H

namespace clone_tools {
	namespace detail {

		/// \brief TODOCUMENT
		struct clone_to_default_tag final {
			clone_to_default_tag()                             = delete;
			~clone_to_default_tag()                            = delete;
			clone_to_default_tag(const clone_to_default_tag &) = delete;
			void operator=(const clone_to_default_tag &)       = delete;
		};

		/// \brief Tag type to indicate the member_clone_type<> type for a type that fails is_member_cloneable_base<>
		struct does_not_member_clone_tag final {
			does_not_member_clone_tag()                                  = delete;
			~does_not_member_clone_tag()                                 = delete;
			does_not_member_clone_tag(const does_not_member_clone_tag &) = delete;
			void operator=(const does_not_member_clone_tag &)            = delete;
		};

	} // namespace common
} // namespace clone_tools

#endif

